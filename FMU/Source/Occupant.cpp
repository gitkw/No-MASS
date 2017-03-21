// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <memory>
#include <cfloat>
#include <cstddef>
#include <utility>

#include "State.h"
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
#include "State_Out.h"
#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"
#include "Occupant.h"

Occupant::Occupant() {}

void Occupant::setup(int id, const agentStruct &agent,
      const std::vector<std::shared_ptr<Building_Zone>> &zones) {
    this->id = id;
    datastoreIdActivity = DataStore::addVariable(idString + "Activity");
    datastoreIdHeatGains = DataStore::addVariable(idString + "HeatGains");
    bedroom = buildingName + agent.bedroom;
    office = buildingName + agent.office;
    power = agent.power;
    if (power == 0) {
      // no power causes problems with agent not present allso having no power
      // so set smallest possible
      power = std::numeric_limits<double>::epsilon();
    }

    for (const std::shared_ptr<Building_Zone> & buldingZone : zones) {
        if (SimulationConfig::info.presencePage &&
            buldingZone->hasActivity(3) &&
            !buldingZone->isNamed(office)) {
          continue;
        }
        if (!SimulationConfig::info.presencePage &&
            buldingZone->hasActivity(0) &&
            !buldingZone->isNamed(bedroom) &&
            buldingZone->getNumberOfActivities() == 1) {
          continue;
        }
        agentZones.push_back(Occupant_Zone());
        agentZones.back().setup(buildingID, *buldingZone, id, agent);
    }
    if (SimulationConfig::info.presencePage) {
      model_presenceFromPage(agent);
    } else {
      model_activity(agent);
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
          !zones[i]->isNamed(office)) {
        continue;
      }
      if (!SimulationConfig::info.presencePage &&
          zones[i]->hasActivity(0) &&
          !zones[i]->isNamed(bedroom) &&
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
      agentZone.stepPre(*zonePtr, *zonePtrPrevious, activities);
    }
    DataStore::addValue(datastoreIdHeatGains,
                                getCurrentRadientGains(*zonePtr));
    DataStore::addValue(datastoreIdActivity, newStateID);
}

void Occupant::model_activity(const agentStruct &agent) {
    Model_Activity_Survival ma;
    ma.setAge(agent.age);
    ma.setComputer(agent.computer);
    ma.setCivstat(agent.civstat);
    ma.setUnemp(agent.unemp);
    ma.setRetired(agent.retired);
    ma.setEdtry(agent.edtry);
    ma.setFamstat(agent.famstat);
    ma.setSex(agent.sex);
    ma.setProbMap(agent.profile);
    activities = ma.preProcessActivities();
}

void Occupant::model_presenceFromPage(const agentStruct &agent) {
    Model_Presence presence;
    presence.setProbMap(agent.profile);
    presence.calculatePresenceFromPage();
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

int Occupant::getStateID() const {
  return state.getId();
}

void Occupant::setBuildingName(const std::string & buildingName) {
  this->buildingName = buildingName;
}
