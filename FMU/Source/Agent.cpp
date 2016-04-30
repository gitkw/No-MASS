// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <memory>
#include <cfloat>
#include <cstddef>

#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"
#include "Agent.h"

Agent::Agent() {
}

Agent::Agent(int newId, const std::vector<Building_Zone> &zones) : id(newId) {
    std::string idAsString = std::to_string(newId);
    DataStore::addVariable("Agent_Activity_" + idAsString);
    DataStore::addVariable("AgentGains" + idAsString);
    agentStruct agent = SimulationConfig::agents.at(id);
    bedroom = agent.bedroom;
    office = agent.office;
    power = agent.power;
    if (power == 0) {
      // no power causes problems with agent not present allso having no power
      // so set smallest possible
      power = DBL_EPSILON;
    }

    for (const Building_Zone & buldingZone : zones) {
        agentZones.push_back(Agent_Zone(buldingZone, id, agent));
    }

    if (agent.HeatOnPresence) {
        availableActions.push_back(4);
    }

    if (SimulationConfig::info.presencePage) {
      model_presenceFromPage();
    } else {
      model_activity();
    }
}

void Agent::step(StateMachine *stateMachine) {
    int stepCount = SimulationConfig::getStepCount();
    int newStateID = activities.at(stepCount);
    previousState = state;
    state = stateMachine->transistionTo(newStateID);

    Building_Zone* zonePtr = state.getZonePtr();
    Building_Zone* zonePtrPrevious = previousState.getZonePtr();

    metabolicRate = state.getMetabolicRate();
    clo = state.getClo();

    // interactWithZone(*zonePtr);
    for (Agent_Zone &agentZone : agentZones) {
      agentZone.setClo(clo);
      agentZone.setMetabolicRate(metabolicRate);
      agentZone.step(*zonePtr, *zonePtrPrevious, activities);
    }


    std::string name = "AgentGains" + std::to_string(id);
    DataStore::addValue(name.c_str(), getCurrentRadientGains(*zonePtr));
    name = "Agent_Activity_" + std::to_string(id);
    DataStore::addValue(name.c_str(), newStateID);
}

void Agent::interactWithZone(const Building_Zone &zone) {
    desires interaction;
  /*
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);

    aah.step(zone, inZone, preZone, activities);
    // interaction->heatState = aah.getResult();
    heatState = aah.getResult();
    */
    // aah.step(zone, inZone, preZone, activities);
    // heatState = aah.getResult();


    zoneToInteraction[zone.getName()] = interaction;
}

void Agent::model_activity() {
    Model_Activity ma;
    activities = ma.preProcessActivities(id);
}

void Agent::model_presenceFromPage() {
    Model_Presence presence;
    presence.calculatePresenceFromPage(id);
    for (unsigned int i = 0; i < presence.size(); ++i) {
        if (presence.at(i)) {
            activities.push_back(3);
        } else {
            activities.push_back(9);
        }
    }
}


double Agent::getCurrentRadientGains(const Building_Zone &zone) const {
    double state = 0.0;
    for (const Agent_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getHeatgains();
        break;
      }
    }

    return state;
}

double Agent::getPower() const {
    return power;
}

bool Agent::getDesiredLightState(const Building_Zone &zone) const {
    bool state = false;
    for (const Agent_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredLightState();
        break;
      }
    }
    return state;
}

bool Agent::getDesiredWindowState(const Building_Zone &zone) const {
    //  return zoneToInteraction.at(zone.getName()).windowState;
    bool state = false;
    for (const Agent_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredWindowState();
        break;
      }
    }
    return state;
}

double Agent::getDesiredShadeState(const Building_Zone &zone) const {
    // return zoneToInteraction.at(zone.getName()).shadeState;
    double state = 1.0;
    for (const Agent_Zone &agentZone : agentZones) {
      if (agentZone.getId() == zone.getId()) {
        state = agentZone.getDesiredShadeState();
        break;
      }
    }
    return state;
}

double Agent::getPMV(const Building_Zone &zone) const {
  double state = 0.0;
  for (const Agent_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      state = agentZone.getPMV();
      break;
    }
  }
  return state;
}

double Agent::getDesiredHeatState(const Building_Zone &zone) const {
  double state = 0.0;
  for (const Agent_Zone &agentZone : agentZones) {
    if (agentZone.getId() == zone.getId()) {
      state = agentZone.getDesiredHeatingSetPoint();
      break;
    }
  }
  return state;
}

bool Agent::currentlyInZone(const Building_Zone &zone) const {
    return zone.getId() == state.getZonePtr()->getId();
}

bool Agent::previouslyInZone(const Building_Zone &zone) const {
    bool inZone = false;
    if (SimulationConfig::getStepCount() > 0) {
        inZone = zone.getId() == previousState.getZonePtr()->getId();
    }
    return inZone;
}

bool Agent::InteractionOnZone(const Building_Zone &zone) const {
  return currentlyInZone(zone) || previouslyInZone(zone);
}

std::string Agent::getLocationType(int step, StateMachine *sm) {
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    // std::cout << "Location is: " << s.getType() <<std::endl;
    return s.getActivity();
}

std::string Agent::getLocationName(int step, StateMachine *sm) {
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    return updateLocation(s);
}

std::string Agent::updateLocation(const State& s) const {
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

void Agent::postprocess() {
  for (Agent_Zone &agentZone : agentZones) {
      agentZone.postprocess();
  }
}

void Agent::setState(State &state) {
  this->state = state;
}
