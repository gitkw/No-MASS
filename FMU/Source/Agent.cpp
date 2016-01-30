// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <memory>

#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"
#include "Agent.h"

Agent::Agent() {
}

Agent::Agent(int newId) : id(newId) {
    std::string idAsString = std::to_string(newId);
    DataStore::addVariable("Agent_Activity_" + idAsString);
    agentStruct agent = SimulationConfig::agents.at(id);
    bedroom = agent.bedroom;
    office = agent.office;
    power = agent.power;

    aahg.setup(id);
    availableActions.push_back(0);

    if (SimulationConfig::info.windows) {
        aaw.setup(agent.windowId);
        aaw.setOpenDuringCooking(agent.WindowOpenDuringCooking);
        aaw.setOpenDuringWashing(agent.WindowOpenDuringWashing);
        availableActions.push_back(1);
    }
    if (SimulationConfig::info.shading) {
        aas.setup(agent.shadeId);
        aas.setClosedDuringSleep(agent.ShadeClosedDuringSleep);
        aas.setClosedDuringWashing(agent.ShadeClosedDuringWashing);
        availableActions.push_back(2);
    }
    if (SimulationConfig::info.lights) {
        availableActions.push_back(3);
        aal.setOffDuringAudioVisual(agent.LightOffDuringAudioVisual);
        aal.setOffDuringSleep(agent.LightOffDuringSleep);
    }
    if (agent.HeatOnPresence) {
        availableActions.push_back(4);
    }

    if (SimulationConfig::info.learn > 0) {
        // availableActions.push_back(5);
        aalearn.setup(id, SimulationConfig::info.learn);
    }

    if (SimulationConfig::info.presencePage) {
      model_presenceFromPage();
    } else {
      model_presenceFromActivities();
    }
}

void Agent::step(StateMachine *stateMachine) {
    int stepCount = SimulationConfig::getStepCount();
    int newStateID = activities.at(stepCount);
    previousState = state;
    state = stateMachine->transistionTo(newStateID);

    if (presence.at(stepCount)) {
        metabolicRate = state.getMetabolicRate();
        clo = state.getClo();
        activity = state.getActivity();
        Zone* zonePtr = state.getZonePtr();
        interactWithZone(*zonePtr);
    }

    if ( stepCount > 0 && previousState.getId() != state.getId()
      && presence.at(stepCount - 1)) {
        metabolicRate = previousState.getMetabolicRate();
        clo = previousState.getClo();
        activity = previousState.getActivity();
        Zone* zonePtr = previousState.getZonePtr();
        interactWithZone(*zonePtr);
    }

    std::string name = "Agent_Activity_" + std::to_string(id);
    DataStore::addValue(name.c_str(), newStateID);
}

void Agent::actionStep(int action, ActionValues *interaction, const Zone &zone,
  bool inZone, bool preZone) {
    switch (action) {
      case 0:
            aahg.prestep(clo, metabolicRate);
            aahg.step(zone, inZone, preZone, activities);
            interaction->heatgains = aahg.getResult();
            previous_pmv = pmv;
            pmv = aahg.getPMV();
        break;
      case 1:
            aaw.step(zone, inZone, preZone, activities);
            interaction->windowState = aaw.getResult();
        break;
      case 2:
            aas.step(zone, inZone, preZone, activities);
            interaction->shadeState = aas.getResult();
        break;
      case 3:
            aal.step(zone, inZone, preZone, activities);
            interaction->lightState = aal.getResult();
        break;
      case 4:
            aah.step(zone, inZone, preZone, activities);
            // interaction->heatState = aah.getResult();
            heatState = aah.getResult();
        break;
      case 5:

        break;
      }
}

void Agent::interactWithZone(const Zone &zone) {
    ActionValues interaction;

    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);

    std::random_shuffle(availableActions.begin(), availableActions.end() );
    for (int a : availableActions) {
        actionStep(a, &interaction, zone, inZone, preZone);
    }

    // aah.step(zone, inZone, preZone, activities);
    // heatState = aah.getResult();

    if (SimulationConfig::info.learn > 0) {
      aalearn.setReward(pmv);
      aalearn.step(zone, inZone, preZone, activities);
      interaction.heatingSetPoint = aalearn.getResult();
    }

    zoneToInteraction[zone.getName()] = interaction;
}

void Agent::model_activity() {
    Model_Activity ma;
    activities = ma.preProcessActivities(id);
}

void Agent::model_presenceFromActivities() {
    model_activity();
    presence.calculatePresenceFromActivities(activities);
}

void Agent::model_presenceFromPage() {
    presence.calculatePresenceFromPage(id);
    for (unsigned int i = 0; i < presence.size(); ++i) {
        if (presence.at(i)) {
            activities.push_back(3);
        } else {
            activities.push_back(9);
        }
    }
}


double Agent::getCurrentRadientGains(const Zone &zone) const {
    return zoneToInteraction.at(zone.getName()).heatgains;
}

double Agent::getPower() const {
    return power;
}

bool Agent::getDesiredLightState(const Zone &zone) const {
    return zoneToInteraction.at(zone.getName()).lightState;
}

bool Agent::getDesiredWindowState(const Zone &zone) const {
    return zoneToInteraction.at(zone.getName()).windowState;
}

double Agent::getDesiredShadeState(const Zone &zone) const {
    return zoneToInteraction.at(zone.getName()).shadeState;
}

double Agent::getDesiredHeatState(const Zone &zone) const {
    return zoneToInteraction.at(zone.getName()).heatingSetPoint;
}

bool Agent::currentlyInZone(const Zone &zone) const {
    return zone.getName() == state.getZonePtr()->getName();
}

bool Agent::previouslyInZone(const Zone &zone) const {
    bool inZone = false;
    if (SimulationConfig::getStepCount() > 0) {
        inZone = zone.getName() == previousState.getZonePtr()->getName();
    }
    return inZone;
}

bool Agent::InteractionOnZone(const Zone &zone) const {
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
  if (SimulationConfig::info.learn > 0) {
    aalearn.print();
  }
}
