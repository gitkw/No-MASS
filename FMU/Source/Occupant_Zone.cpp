// Copyright 2016 Jacob Chapman

#include <iostream>
#include <vector>
#include <algorithm>

#include "Environment.h"
#include "DataStore.h"
#include "Utility.h"
#include "Occupant_Zone.h"

Occupant_Zone::Occupant_Zone() {}

bool Occupant_Zone::isActionWindow() const {
  return ActionWindow;
}
bool Occupant_Zone::isActionLights() const {
  return ActionLights;
}
bool Occupant_Zone::isActionShades() const {
  return ActionShades;
}
bool Occupant_Zone::isActionHeatGains() const {
  return ActionHeatGains;
}
bool Occupant_Zone::isActionLearning() const {
  return ActionLearning;
}

void Occupant_Zone::setup(int buildingID, const Building_Zone & buldingZone,
                        int agentid, const agentStruct &agent) {
  id = buldingZone.getId();
  this->buildingID = buildingID;
  if (SimulationConfig::info.agentHeatGains) {
      aahg.setup(buildingID, agentid);
      availableActions.push_back(0);
  }
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

void Occupant_Zone::setupLights(const agentStruct &agent,
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

void Occupant_Zone::setupShades(const agentStruct &agent,
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

void Occupant_Zone::setupWindows(int agentid, const agentStruct &agent,
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

void Occupant_Zone::shuffleActions() {
  std::shuffle(availableActions.begin(), availableActions.end(), Utility::engine);
}

void Occupant_Zone::step(const Building_Zone& zone,
                      const Building_Zone& zonePrevious,
                      const std::vector<double> &activities) {
    bool inZone = zone.getId() == id;
    bool previouslyInZone = zonePrevious.getId() == id;
    if (isInBuilding()) {
      if (inZone) {
        shuffleActions();
        for (int a : availableActions) {
            actionStep(a, zone, inZone, previouslyInZone, activities);
        }
      }
      if (SimulationConfig::info.heating && SimulationConfig::info.learn > 0) {
        actionStep(5, zone, inZone, previouslyInZone, activities);
      }
    }
    BDI(activities);
}

void Occupant_Zone::stepPre(const Building_Zone& zone,
                      const Building_Zone& zonePrevious,
                      const std::vector<double> &activities) {
    bool inZone = zone.getId() == id;
    bool previouslyInZone = zonePrevious.getId() == id;
    if ( !inZone && previouslyInZone && isInBuilding()) {
        shuffleActions();
        for (int a : availableActions) {
            actionStep(a, zonePrevious, inZone, previouslyInZone, activities);
        }
    }
    if (inZone || previouslyInZone) {
        if (SimulationConfig::info.windows){
            aaw.saveResult();
        }
    }
}

void Occupant_Zone::BDI(const std::vector<double> &activities) {
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

void Occupant_Zone::actionStep(int action,
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
            {
              ActionWindow = true;
              double dailyMeanTe = Environment::getDailyMeanTemperature();
              aaw.setDailyMeanTemperature(dailyMeanTe);
              aaw.step(zone, inZone, preZone, activities);
              desiredWindowState = aaw.getResult();
            }
        break;
      case 2:
            {
              ActionShades = true;
              double ill = zone.getDaylightingReferencePoint1Illuminance();
              aas.setIndoorIlluminance(ill);
              aas.step(zone, inZone, preZone);
              desiredShadeState = aas.getResult();
            }
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
            {
              ActionWindow = true;
              double dailyMeanTe = Environment::getDailyMeanTemperature();
              aawBDI.setDailyMeanTemperature(dailyMeanTe);
              aawBDI.step(zone, inZone, preZone, activities);
              desiredWindowState = aawBDI.getResult();
            }
        break;
      case 7:
            ActionLights = true;
            aalBDI.step(zone, inZone, preZone, activities);
            desiredLightState = aalBDI.getResult();
        break;
      case 8:
            {
              ActionShades = true;
              double ill = zone.getDaylightingReferencePoint1Illuminance();
              aasBDI.setIndoorIlluminance(ill);
              aasBDI.step(zone, inZone, preZone);
              desiredShadeState = aasBDI.getResult();
            }
        break;
      }
}

int Occupant_Zone::getId() const {
  return id;
}

bool Occupant_Zone::getDesiredWindowState() const {
  return desiredWindowState;
}

bool Occupant_Zone::getDesiredLightState() const {
  return desiredLightState;
}

double Occupant_Zone::getPMV() const {
  return pmv;
}

double Occupant_Zone::getHeatgains() const {
  return heatgains;
}

double Occupant_Zone::getDesiredHeatingSetPoint() const {
  return desiredHeatingSetPoint;
}

double Occupant_Zone::getDesiredShadeState() const {
  return desiredShadeState;
}

void Occupant_Zone::setClo(double clo) {
  this->clo = clo;
}

void Occupant_Zone::setMetabolicRate(double metabolicRate) {
  this->metabolicRate = metabolicRate;
}

void Occupant_Zone::postprocess() {
  if (isInBuilding() && SimulationConfig::info.learn > 0) {
    aalearn.print();
    aalearn.reset();
  }
  if (isInBuilding() && SimulationConfig::info.windowsLearn > 0) {
    aawLearn.print();
    aawLearn.reset();
  }
}

void Occupant_Zone::postTimeStep() {
  ActionWindow = false;
  ActionLights = false;
  ActionShades = false;
  ActionHeatGains = false;
  ActionLearning = false;
}

bool Occupant_Zone::isInBuilding() const {
    return id > 0;  // 0 is the ID for the outside zone
}

int Occupant_Zone::getDesiredWindowDuration() const {
  return aaw.durationOpen();
}

double Occupant_Zone::getDesiredAppliance() const {
  return desiredApplianceState;
}

bool Occupant_Zone::isActionAppliance() const {
  return ActionAppliance;
}

void Occupant_Zone::disableBDI() {
  hasBDI = false;
}

void Occupant_Zone::enableBDI() {
  hasBDI = true;
}
