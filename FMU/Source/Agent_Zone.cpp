// Copyright 2016 Jacob Chapman

#include <iostream>
#include <vector>
#include <algorithm>

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

Agent_Zone::Agent_Zone(const Building_Zone & buldingZone, int agentid,
                        const agentStruct &agent) {
  id = buldingZone.getId();
  aahg.setup(agentid);
  availableActions.push_back(0);

  ActionWindow = false;
  ActionLights = false;
  ActionShades = false;
  ActionHeatGains = false;
  ActionLearning = false;

  if (SimulationConfig::info.windows) {
      aaw.setup(agent.windowId, agentid);
      if (buldingZone.hasActivity(4)) {
        aaw.setOpenDuringCooking(agent.WindowOpenDuringCooking);
      }
      if (buldingZone.hasActivity(6)) {
        aaw.setOpenDuringWashing(agent.WindowOpenDuringWashing);
      }
      if (buldingZone.hasActivity(0)) {
        aaw.setOpenDuringSleeping(agent.WindowOpenDuringSleeping);
      }
      aaw.setAvailableActivities(buldingZone.getActivities());
      availableActions.push_back(1);
  } else if (SimulationConfig::info.windowsLearn) {
      aawLearn.setZoneId(id);
      aawLearn.setup(agent.windowId, agentid);
      availableActions.push_back(4);
  }

  if (SimulationConfig::info.lights) {
      availableActions.push_back(3);
      if (buldingZone.hasActivity(2)) {
        aal.setOffDuringAudioVisual(agent.LightOffDuringAudioVisual);
      }
      aal.setOffDuringSleep(agent.LightOffDuringSleep);
      aal.setAvailableActivities(buldingZone.getActivities());
  }
  if (SimulationConfig::info.shading) {
      aas.setup(agent.shadeId);
      if (buldingZone.hasActivity(0)) {
        aas.setClosedDuringSleep(agent.ShadeClosedDuringSleep);
      }
      if (buldingZone.hasActivity(6)) {
        aas.setClosedDuringWashing(agent.ShadeClosedDuringWashing);
      }
      aas.setClosedDuringNight(agent.ShadeDuringNight);
      if (buldingZone.hasActivity(2)) {
        aas.setClosedDuringAudioVisual(agent.ShadeDuringAudioVisual);
      }
      availableActions.push_back(2);
  }
  if (SimulationConfig::info.learn > 0) {
      aalearn.setZoneId(id);
      aalearn.setup(agentid, SimulationConfig::info.learn);
  }
}

void Agent_Zone::step(const Building_Zone& zone,
                      const Building_Zone& zonePrevious,
                      const std::vector<double> &activities) {
    double outdoorTemperature =
            DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");
    outDoorTemperatures.push_back(outdoorTemperature);
    if (outDoorTemperatures.size() >
          (SimulationConfig::info.timeStepsPerHour * 24)) {
            outDoorTemperatures.pop_front();
    }
    dailyMeanTemperature = 0;
    for (double temp : outDoorTemperatures) {
            dailyMeanTemperature += temp;
    }
    dailyMeanTemperature =
      dailyMeanTemperature /
        static_cast<double>(outDoorTemperatures.size());
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
    bool win = aaw.BDI(activities);
    if (win) {
      desiredWindowState = aaw.getResult();
      ActionWindow = true;
    }

    bool sha = aas.BDI(activities);
    if (sha) {
      desiredShadeState = aas.getResult();
      ActionShades = true;
    }

    bool lig = aal.BDI(activities);
    if (lig) {
      desiredLightState = aal.getResult();
      ActionLights = true;
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
            aaw.setDailyMeanTemperature(dailyMeanTemperature);
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
            aawLearn.setReward(pmv);
            aawLearn.step(zone, inZone);
            desiredWindowState = aawLearn.getResult();
            ActionWindow = true;
        break;
      case 5:
            ActionLearning = true;
            aalearn.setReward(pmv);
            aalearn.step(zone, inZone);
            desiredHeatingSetPoint = aalearn.getResult();
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
