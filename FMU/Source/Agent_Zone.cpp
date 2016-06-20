// Copyright 2016 Jacob Chapman

#include <iostream>
#include <vector>
#include <algorithm>

#include "Building.h"
#include "DataStore.h"
#include "Agent_Zone.h"

Agent_Zone::Agent_Zone() {}

bool Agent_Zone::isActionWindow() const {
  return ActionWindow;
}
bool Agent_Zone::isActionLights() const {
  return ActionLights;
}
bool Agent_Zone::isActionShades() const {
  return ActionShades;
}
bool Agent_Zone::isActionHeatGains() const {
  return ActionHeatGains;
}
bool Agent_Zone::isActionLearning() const {
  return ActionLearning;
}

void Agent_Zone::setup(const Building_Zone & buldingZone, int agentid,
                        const agentStruct &agent) {
  id = buldingZone.getId();

  aahg.setup(agentid);
  availableActions.push_back(0);
  disableBDI();

  ActionHeatGains = false;
  ActionLearning = false;

  setupWindows(agentid, agent, buldingZone);
  setupLights(agent, buldingZone);
  setupShades(agent, buldingZone);

  if (SimulationConfig::info.learn > 0) {
      aalearn.setZoneId(id);
      aalearn.setup(agentid, SimulationConfig::info.learn);
  }
  if (agent.ApplianceDuringDay > 0) {
    aaa.setApplianceDuringDay(agent.ApplianceDuringDay);
    enableBDI();
  }
}


void Agent_Zone::setupLights(const agentStruct &agent,
    const Building_Zone & buldingZone) {
      ActionLights = false;

      if ((agent.LightOffDuringAudioVisual > 0 ||
          agent.LightOffDuringSleep > 0) &&
          SimulationConfig::info.lights
        ) {
          availableActions.push_back(7);
          if (buldingZone.hasActivity(2)) {
            aalBDI.setOffDuringAudioVisual(agent.LightOffDuringAudioVisual);
          }
          aalBDI.setOffDuringSleep(agent.LightOffDuringSleep);
          aalBDI.setAvailableActivities(buldingZone.getActivities());
          enableBDI();
      } else if (SimulationConfig::info.lights) {
          availableActions.push_back(3);
          aal.setAvailableActivities(buldingZone.getActivities());
      }
}

void Agent_Zone::setupShades(const agentStruct &agent,
    const Building_Zone & buldingZone) {
    ActionShades = false;
    if ((agent.ShadeClosedDuringSleep > 0 ||
        agent.ShadeDuringNight > 0 ||
        agent.ShadeDuringAudioVisual > 0 ||
        agent.ShadeClosedDuringWashing > 0)
      && SimulationConfig::info.shading) {
      aasBDI.setup(agent.shadeId);
      if (buldingZone.hasActivity(0)) {
        aasBDI.setClosedDuringSleep(agent.ShadeClosedDuringSleep);
      }
      if (buldingZone.hasActivity(6)) {
        aasBDI.setClosedDuringWashing(agent.ShadeClosedDuringWashing);
      }
      aasBDI.setClosedDuringNight(agent.ShadeDuringNight);
      if (buldingZone.hasActivity(2)) {
        aasBDI.setClosedDuringAudioVisual(agent.ShadeDuringAudioVisual);
      }
      availableActions.push_back(8);
      enableBDI();
    } else if (SimulationConfig::info.shading) {
          aas.setup(agent.shadeId);
          availableActions.push_back(2);
      }
}

void Agent_Zone::setupWindows(int agentid, const agentStruct &agent,
    const Building_Zone & buldingZone) {
  ActionWindow = false;

  if ((agent.WindowOpenDuringCooking > 0 ||
      agent.WindowOpenDuringWashing > 0 ||
      agent.WindowOpenDuringSleeping > 0) &&
      SimulationConfig::info.windows) {
    enableBDI();
    aawBDI.setup(agent.windowId, agentid);
    if (buldingZone.hasActivity(4)) {
      aawBDI.setOpenDuringCooking(agent.WindowOpenDuringCooking);
    }
    if (buldingZone.hasActivity(6)) {
      aawBDI.setOpenDuringWashing(agent.WindowOpenDuringWashing);
    }
    if (buldingZone.hasActivity(0)) {
      aawBDI.setOpenDuringSleeping(agent.WindowOpenDuringSleeping);
    }
    aawBDI.setAvailableActivities(buldingZone.getActivities());
    availableActions.push_back(6);

  } else if (SimulationConfig::info.windows) {
    aaw.setup(agent.windowId, agentid);
    aaw.setAvailableActivities(buldingZone.getActivities());
    availableActions.push_back(1);
  } else if (SimulationConfig::info.windowsLearn) {
    aawLearn.setZoneId(id);
    aawLearn.setup(agentid);
    availableActions.push_back(4);
  }
}

void Agent_Zone::step(const Building_Zone& zone,
                      const Building_Zone& zonePrevious,
                      const std::vector<double> &activities) {
    bool inZone = zone.getId() == id;
    bool previouslyInZone = zonePrevious.getId() == id;
    if (inZone || previouslyInZone) {
      aaw.saveResult();
    }

    if (isInBuilding()) {
      if (inZone || previouslyInZone) {
        std::random_shuffle(availableActions.begin(), availableActions.end() );
        for (int a : availableActions) {
          if (inZone) {
            actionStep(a, zone, inZone, previouslyInZone, activities);
          } else if (previouslyInZone) {
            actionStep(a, zonePrevious, inZone, previouslyInZone, activities);
          }
        }
      }
      if (SimulationConfig::info.learn > 0) {
        actionStep(5, zone, inZone, previouslyInZone, activities);
      }
    }
    BDI(activities);
}

void Agent_Zone::BDI(const std::vector<double> &activities) {
  if (hasBDI) {
    if (aawBDI.doRecipe(activities)) {
      desiredWindowState = aawBDI.getResult();
      ActionWindow = true;
    }

    if (aasBDI.doRecipe(activities)) {
      desiredShadeState = aasBDI.getResult();
      ActionShades = true;
    }

    if (aalBDI.doRecipe(activities)) {
      desiredLightState = aalBDI.getResult();
      ActionLights = true;
    }

    if (aaa.doRecipe(activities)) {
      desiredApplianceState = aaa.getResult();
      ActionAppliance = true;
    }
  }
}

void Agent_Zone::actionStep(int action,
  const Building_Zone &zone, bool inZone, bool preZone,
  const std::vector<double> &activities) {
    switch (action) {
      case 0:
            if (inZone) {
              ActionHeatGains = true;
              aahg.prestep(clo, metabolicRate);
              aahg.step(zone, inZone);
              heatgains = aahg.getResult();
              previous_pmv = pmv;
              pmv = aahg.getPMV();
              ppd = aahg.getPPD();
            }

        break;
      case 1:
            ActionWindow = true;
            aaw.setDailyMeanTemperature(Building::dailyMeanTemperature);
            aaw.step(zone, inZone, preZone, activities);
            desiredWindowState = aaw.getResult();
        break;
      case 2:
            ActionShades = true;
            aas.step(zone, inZone, preZone);
            desiredShadeState = aas.getResult();
        break;
      case 3:
            ActionLights = true;
            aal.step(zone, inZone, preZone, activities);
            desiredLightState = aal.getResult();
        break;
      case 4:
            ActionWindow = true;
            aawLearn.setReward(pmv);
            aawLearn.step(zone, inZone, preZone);
            desiredWindowState = aawLearn.getResult();
        break;
      case 5:
            ActionLearning = true;
            aalearn.setReward(pmv);
            aalearn.step(zone, inZone);
            desiredHeatingSetPoint = aalearn.getResult();
        break;
      case 6:
            ActionWindow = true;
            aawBDI.setDailyMeanTemperature(Building::dailyMeanTemperature);
            aawBDI.step(zone, inZone, preZone, activities);
            desiredWindowState = aawBDI.getResult();
        break;
      case 7:
            ActionLights = true;
            aalBDI.step(zone, inZone, preZone, activities);
            desiredLightState = aalBDI.getResult();
        break;
      case 8:
            ActionShades = true;
            aasBDI.step(zone, inZone, preZone);
            desiredShadeState = aasBDI.getResult();
        break;
      }
}

int Agent_Zone::getId() const {
  return id;
}

bool Agent_Zone::getDesiredWindowState() const {
  return desiredWindowState;
}

bool Agent_Zone::getDesiredLightState() const {
  return desiredLightState;
}

double Agent_Zone::getPMV() const {
  return pmv;
}

double Agent_Zone::getHeatgains() const {
  return heatgains;
}

double Agent_Zone::getDesiredHeatingSetPoint() const {
  return desiredHeatingSetPoint;
}

double Agent_Zone::getDesiredShadeState() const {
  return desiredShadeState;
}

void Agent_Zone::setClo(double clo) {
  this->clo = clo;
}

void Agent_Zone::setMetabolicRate(double metabolicRate) {
  this->metabolicRate = metabolicRate;
}

void Agent_Zone::postprocess() {
  if (isInBuilding() && SimulationConfig::info.learn > 0) {
    aalearn.print();
    aalearn.reset();
  }
  if (isInBuilding() && SimulationConfig::info.windowsLearn > 0) {
    aawLearn.print();
    aawLearn.reset();
  }
}

void Agent_Zone::postTimeStep() {
  ActionWindow = false;
  ActionLights = false;
  ActionShades = false;
  ActionHeatGains = false;
  ActionLearning = false;
}

bool Agent_Zone::isInBuilding() const {
    return id > 0;  // 0 is the ID for the outside zone
}

int Agent_Zone::getDesiredWindowDuration() const {
  return aaw.durationOpen();
}

double Agent_Zone::getDesiredAppliance() const {
  return desiredApplianceState;
}

bool Agent_Zone::isActionAppliance() const {
  return ActionAppliance;
}

void Agent_Zone::disableBDI() {
  hasBDI = false;
}

void Agent_Zone::enableBDI() {
  hasBDI = true;
}
