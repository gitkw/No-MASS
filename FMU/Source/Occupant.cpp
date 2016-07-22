// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <memory>
#include <cfloat>
#include <cstddef>

#include "State_Out.h"
#include "State_Present.h"
#include "State_Sleep.h"
#include "State_Passive.h"
#include "State_IT.h"
#include "State_Washing_Appliance.h"
#include "State_Audio_Visual.h"
#include "State_Cleaning.h"
#include "State_Cooking.h"
#include "State_Washing.h"
#include "State_Metabolic.h"
#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"
#include "Occupant.h"

Occupant::Occupant() {}

void Occupant::setup(int newId,
      const std::vector<std::shared_ptr<Building_Zone>> &zones) {
    id = newId;
    std::string idAsString = std::to_string(newId);
    DataStore::addVariable("Occupant_Activity_" + idAsString);
    DataStore::addVariable("AgentGains" + idAsString);
    agentStruct agent = SimulationConfig::buildings[buildingID].agents.at(id);
    buildingName = SimulationConfig::buildings[buildingID].name;
    bedroom = buildingName + agent.bedroom;
    office = buildingName + agent.office;
    power = agent.power;
    if (power == 0) {
      // no power causes problems with agent not present allso having no power
      // so set smallest possible
      power = DBL_EPSILON;
    }

    for (const std::shared_ptr<Building_Zone> & buldingZone : zones) {
        if (SimulationConfig::info.presencePage &&
            buldingZone->hasActivity(3) &&
            buldingZone->getName() != office) {
          continue;
        }
        if (!SimulationConfig::info.presencePage &&
            buldingZone->hasActivity(0) &&
            buldingZone->getName() != bedroom &&
            buldingZone->getNumberOfActivities() == 1) {
          continue;
        }
        agentZones.push_back(Occupant_Zone());
        agentZones.back().setup(*buldingZone, id, agent);
    }
    if (SimulationConfig::info.presencePage) {
      model_presenceFromPage();
    } else {
      model_activity();
    }
    initialiseStates(zones);
}

void Occupant::initialiseStates(
                  const std::vector<std::shared_ptr<Building_Zone>> &zones) {
    State_Present present;
    if (SimulationConfig::info.presencePage) {
        State_IT it;
        matchStateToZone(&it, zones);
        present.addState(it);
    } else {
        State_Sleep sleep;
        matchStateToZone(&sleep, zones);
        State_Passive passive;
        matchStateToZone(&passive, zones);
        State_Washing_Appliance washingAppliance;
        matchStateToZone(&washingAppliance, zones);
        State_Washing washing;
        matchStateToZone(&washing, zones);
        State_Audio_Visual audioVisual;
        matchStateToZone(&audioVisual, zones);
        State_Cleaning cleaning;
        matchStateToZone(&cleaning, zones);
        State_Cooking cooking;
        matchStateToZone(&cooking, zones);
        State_Metabolic metabolic;
        matchStateToZone(&metabolic, zones);
        State_IT it;
        matchStateToZone(&it, zones);
        present.addState(sleep);
        present.addState(passive);
        present.addState(washingAppliance);
        present.addState(washing);
        present.addState(audioVisual);
        present.addState(cleaning);
        present.addState(cooking);
        present.addState(metabolic);
        present.addState(it);
    }
    stateMachine.addState(present);
    State_Out out;
    matchStateToZone(&out, zones);
    stateMachine.addState(out);
    setState(out);
}

void Occupant::matchStateToZone(State *s,
                  const std::vector<std::shared_ptr<Building_Zone>> &zones) {
    for (unsigned int i =0; i < zones.size(); i++) {
      if (SimulationConfig::info.presencePage &&
          zones[i]->hasActivity(3) &&
          zones[i]->getName() != office) {
        continue;
      }
      if (!SimulationConfig::info.presencePage &&
          zones[i]->hasActivity(0) &&
          zones[i]->getName() != bedroom &&
          zones[i]->getNumberOfActivities() == 1) {
        continue;
      }
      if (zones[i]->hasActivity(s->getId())) {
          s->setZonePtr(zones[i]);
          break;
      }
    }
}

void Occupant::step() {
    int stepCount = SimulationConfig::getStepCount();
    int newStateID = activities.at(stepCount);
    zonePtrPrevious = state.getZonePtr();
    state = stateMachine.transistionTo(newStateID);

    std::shared_ptr<Building_Zone> zonePtr = state.getZonePtr();

    metabolicRate = state.getMetabolicRate();
    clo = state.getClo();
    for (Occupant_Zone &agentZone : agentZones) {
      agentZone.setClo(clo);
      agentZone.setMetabolicRate(metabolicRate);
      agentZone.step(*zonePtr, *zonePtrPrevious, activities);
    }
    std::string name = "AgentGains" + std::to_string(id);
    DataStore::addValue(name.c_str(), getCurrentRadientGains(*zonePtr));
    name = "Occupant_Activity_" + std::to_string(id);
    DataStore::addValue(name.c_str(), newStateID);
}

void Occupant::model_activity() {
    Model_Activity ma;
    activities = ma.preProcessActivities(buildingID, id);
}

void Occupant::model_presenceFromPage() {
    Model_Presence presence;
    presence.calculatePresenceFromPage(buildingID, id);
    for (unsigned int i = 0; i < presence.size(); ++i) {
        if (presence.at(i)) {
            activities.push_back(3);
        } else {
            activities.push_back(9);
        }
    }
}


double Occupant::getCurrentRadientGains(const Building_Zone &zone) const {
    double state = 0.0;
    for (const Occupant_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getHeatgains();
        break;
      }
    }

    return state;
}

double Occupant::getPower() const {
    return power;
}

bool Occupant::getDesiredLightState(const Building_Zone &zone) const {
    bool state = false;
    for (const Occupant_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredLightState();
        break;
      }
    }
    return state;
}

bool Occupant::getDesiredWindowState(const Building_Zone &zone) const {
    bool state = false;
    for (const Occupant_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredWindowState();
        break;
      }
    }
    return state;
}

double Occupant::getDesiredShadeState(const Building_Zone &zone) const {
    double state = 1.0;
    for (const Occupant_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredShadeState();
        break;
      }
    }
    return state;
}

double Occupant::getDesiredAppliance(const Building_Zone &zone) const {
  double state = 1.0;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      state = agentZone.getDesiredAppliance();
      break;
    }
  }
  return state;
}

double Occupant::getPMV(const Building_Zone &zone) const {
  double state = 0.0;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      state = agentZone.getPMV();
      break;
    }
  }
  return state;
}

double Occupant::getDesiredHeatState(const Building_Zone &zone) const {
  double state = 0.0;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      state = agentZone.getDesiredHeatingSetPoint();
      break;
    }
  }
  return state;
}

bool Occupant::currentlyInZone(const Building_Zone &zone) const {
    return zone.getId() == state.getZonePtr()->getId();
}

bool Occupant::previouslyInZone(const Building_Zone &zone) const {
    bool inZone = false;
    if (SimulationConfig::getStepCount() > 0) {
        inZone = zone.getId() == zonePtrPrevious->getId();
    }
    return inZone;
}

bool Occupant::InteractionOnZone(const Building_Zone &zone) const {
  return currentlyInZone(zone) || previouslyInZone(zone);
}

std::string Occupant::getLocationType(int step, StateMachine *sm) {
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    return s.getActivity();
}

std::string Occupant::getLocationName(int step, StateMachine *sm) {
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    return updateLocation(s);
}

std::string Occupant::updateLocation(const State& s) const {
    std::string tempLocation = s.getLocation();
    if (s.getActivity() == "Sleeping") {
        tempLocation = bedroom;
    } else if (s.getActivity() == "IT" && SimulationConfig::info.presencePage) {
        if (office != "") {
            tempLocation = office;
        }
    }
    return tempLocation;
}

void Occupant::postprocess() {
  for (Occupant_Zone &agentZone : agentZones) {
      agentZone.postprocess();
  }
}

void Occupant::setState(const State &state) {
  this->state = state;
}

bool Occupant::isActionWindow(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionWindow();
      break;
    }
  }
  return act;
}

bool Occupant::isActionLights(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionLights();
      break;
    }
  }
  return act;
}
bool Occupant::isActionShades(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionShades();
      break;
    }
  }
  return act;
}
bool Occupant::isActionHeatGains(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionHeatGains();
      break;
    }
  }
  return act;
}
bool Occupant::isActionLearning(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionLearning();
      break;
    }
  }
  return act;
}

bool Occupant::isActionAppliance(const Building_Zone &zone) const {
  bool act = false;
  for (const Occupant_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      act = agentZone.isActionAppliance();
      break;
    }
  }
  return act;
}

void Occupant::postTimeStep() {
  for (Occupant_Zone &agentZone : agentZones) {
    agentZone.postTimeStep();
  }
}
