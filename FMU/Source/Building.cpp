// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Agent.h"
#include "Utility.h"
#include "Model_Presence.h"
#include "Building.h"

double Building::dailyMeanTemperature = 0;

Building::Building() {}

void Building::setup(const buildingStruct &b) {
    name = b.name;
    for (std::pair<std::string, ZoneStruct> z : b.zones) {
      zones.push_back(std::make_shared<Building_Zone>(Building_Zone()));
      zones.back()->setName(name + z.second.name);
      zones.back()->setup(z.second);
    }
    int popSize = SimulationConfig::numberOfAgents();
    std::list<int> pop = Utility::randomIntList(popSize);
    // setup each agent randomly
    for (int a : pop) {
          population.push_back(Agent());
          population.back().setBuilding(name);
          population.back().setup(a, zones);
    }
}

void Building::setZones(
        const std::vector<std::shared_ptr<Building_Zone>> & zones) {
    this->zones = zones;
}


void Building::step() {
    double outdoorTemperature =
            DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");
    outDoorTemperatures.push_back(outdoorTemperature);
    if (outDoorTemperatures.size() >
          (SimulationConfig::info.timeStepsPerHour * 24)) {
            outDoorTemperatures.pop_front();
    }
    dailyMeanTemperature = 0;
    for (const double temp : outDoorTemperatures) {
            dailyMeanTemperature += temp;
    }
    dailyMeanTemperature =
      dailyMeanTemperature /
        static_cast<double>(outDoorTemperatures.size());



    int popSize = population.size();
    std::list<int> pop = Utility::randomIntList(popSize);
    for (int a : pop) {
        population[a].step();
    }
    for (std::shared_ptr<Building_Zone> &zone : zones) {
        if (!zone->isActive()) {
            continue;
        }
        setAgentCountForZone(zone);

        if (SimulationConfig::info.windows
          || SimulationConfig::info.windowsLearn) {
            setAgentWindowDecisionForZone(zone);
        }
        if (SimulationConfig::info.shading) {
            setAgentBlindDecisionForZone(zone);
        }
        if (SimulationConfig::info.lights) {
            setAgentLightDecisionForZone(zone);
        }
        setAgentHeatDecisionsForZone(zone);
        setAgentGainsForZone(zone);
        setAppGainsForZone(zone);
    }

    buildingInteractions();
    for (std::shared_ptr<Building_Zone> &zone : zones) {
      if (zone->isActive()) {
        zone->step();
      }
    }
}

void Building::buildingInteractions() {
  if (SimulationConfig::info.ShadeClosedDuringNight) {
    int hourOfDay = DataStore::getValue("hourOfDay");
    if (hourOfDay > 19 || hourOfDay < 6) {
      for (std::shared_ptr<Building_Zone> &zone : zones) {
        if (zone->isActive()) {
          zone->setBlindState(0);
        }
      }
    }
  }
}

void Building::setAgentHeatDecisionsForZone(
      std::shared_ptr<Building_Zone> zone) {
  double currentState = zone->getHeatingState();
  double totalIncrease = 0;
  double totalDecrease = 0;
  double increase = 0;
  double decrease = 0;
  double same = 0;
  double increasePower = 0;
  double decreasePower = 0;
  double samePower = 0;

  for (Agent &agent : population) {
    //  if (agent.InteractionOnZone(*zone)) {
          double d = agent.getDesiredHeatState(*zone);
          double power = agent.getPower();

          if (d < currentState) {
              decrease++;
              totalDecrease = totalDecrease + d;
              decreasePower = decreasePower + power;

          } else if (d > currentState) {
              increase++;
              totalIncrease = totalIncrease + d;
              increasePower = increasePower + power;
          } else {
              same++;
              samePower = samePower + power;
          }
      //}
  }

  double state = currentState;
  if (samePower > increasePower && samePower > decreasePower) {
      state = currentState;
  } else if (samePower < increasePower && increasePower > decreasePower) {
      state = totalIncrease / increase;
  } else if (samePower < decreasePower && increasePower < decreasePower) {
      state = totalDecrease / decrease;
  } else if (samePower == increasePower && samePower > decreasePower) {
      if (Utility::tossACoin()) {
          state = totalIncrease / increase;
      }
  } else if (samePower > increasePower && samePower == decreasePower) {
      if (Utility::tossACoin()) {
          state = totalDecrease / decrease;
      }
  } else if (samePower < increasePower
      && samePower < decreasePower
      && increasePower == decreasePower) {
      if (Utility::tossACoin()) {
          state = totalIncrease / increase;
      } else {
          state = totalDecrease / decrease;
      }
  } else if ((samePower == increasePower) == decreasePower) {
      double i = Utility::randomDouble(0, 1);
      double d = Utility::randomDouble(0, 1);
      double s = Utility::randomDouble(0, 1);
      if (i > d && i > s) {
          state = totalIncrease / increase;
      } else if (d > s && d > i) {
          state = totalDecrease / decrease;
      } else if (s > i && s > d) {
          state = currentState;
      }
  }
  zone->setHeatingState(state);
}

void Building::setAgentGainsForZone(std::shared_ptr<Building_Zone> zone) {
    double numberOfAgents = 0;
    double totalRadientGains = 0;
    double aveRadientGains = 0;

    for (Agent &agent : population) {
        if  (agent.isActionHeatGains(*zone)) {
            numberOfAgents++;
            totalRadientGains += agent.getCurrentRadientGains(*zone);
        }
    }
    if (totalRadientGains > 0) {
        aveRadientGains = totalRadientGains / numberOfAgents;
    }
    zone->setCurrentAgentGains(aveRadientGains);
}

void Building::setAppGainsForZone(std::shared_ptr<Building_Zone> zone) {
    double totalRadientGainsAppliance = 0;
    double aveRadientGains = 0;
    double numberOfAgentsAppliance = 0;

    for (Agent &agent : population) {
        numberOfAgentsAppliance++;
        if  (agent.isActionAppliance(*zone)) {
            totalRadientGainsAppliance += agent.getDesiredAppliance(*zone);
        }
    }
    if (totalRadientGainsAppliance > 0) {
        aveRadientGains += totalRadientGainsAppliance / numberOfAgentsAppliance;
    }
    zone->setAppFraction(aveRadientGains);
}

void Building::setAgentWindowDecisionForZone(
        std::shared_ptr<Building_Zone> zone) {
    double open = 0;
    double close = 0;
    int numberOfActiveAgents = 0;

    for (Agent &agent : population) {
        if (agent.isActionWindow(*zone)) {
            double power = agent.getPower();
          //  if ( power < 2 ) continue;
            numberOfActiveAgents++;
            if (agent.getDesiredWindowState(*zone)) {
                open = open + power;
            } else {
                close = close + power;
            }
        }
    }
    if (numberOfActiveAgents > 0) {
        if (open < close) {
            zone->setWindowState(false);
        } else if (open > close) {
            zone->setWindowState(true);
        } else {
            zone->setWindowState(Utility::tossACoin());
                zone->setWindowState(true);
        }
    }
}

void Building::setAgentBlindDecisionForZone(
        std::shared_ptr<Building_Zone> zone) {
    double currentState = zone->getBlindState();
    double totalIncrease = 0;
    double totalDecrease = 0;
    double increase = 0;
    double decrease = 0;
    double same = 0;
    double increasePower = 0;
    double decreasePower = 0;
    double samePower = 0;

    for (Agent &agent : population) {
        if (agent.isActionShades(*zone)) {
            double d = agent.getDesiredShadeState(*zone);
            double power = agent.getPower();

            if (d < currentState) {
                decrease++;
                totalDecrease = totalDecrease + d;
                decreasePower = decreasePower + power;

            } else if (d > currentState) {
                increase++;
                totalIncrease = totalIncrease + d;
                increasePower = increasePower + power;
            } else {
                same++;
                samePower = samePower + power;
            }
        }
    }

    double state = currentState;
    if (samePower > increasePower && samePower > decreasePower) {
        state = currentState;
    } else if (samePower < increasePower && increasePower > decreasePower) {
        state = totalIncrease / increase;
    } else if (samePower < decreasePower && increasePower < decreasePower) {
        state = totalDecrease / decrease;
    } else if (samePower == increasePower && samePower > decreasePower) {
        if (Utility::tossACoin()) {
            state = totalIncrease / increase;
        }
    } else if (samePower > increasePower && samePower == decreasePower) {
        if (Utility::tossACoin()) {
            state = totalDecrease / decrease;
        }
    } else if (samePower < increasePower
        && samePower < decreasePower
        && increasePower == decreasePower) {
        if (Utility::tossACoin()) {
            state = totalIncrease / increase;
        } else {
            state = totalDecrease / decrease;
        }
    } else if ((samePower == increasePower) == decreasePower) {
        double i = Utility::randomDouble(0, 1);
        double d = Utility::randomDouble(0, 1);
        double s = Utility::randomDouble(0, 1);
        if (i > d && i > s) {
            state = totalIncrease / increase;
        } else if (d > s && d > i) {
            state = totalDecrease / decrease;
        } else if (s > i && s > d) {
            state = currentState;
        }
    }
    zone->setBlindState(state);
}
void Building::setAgentLightDecisionForZone(
        std::shared_ptr<Building_Zone> zone) {
    double open = 0;
    double close = 0;
    double numberOfActiveAgents = 0;
    for (Agent &agent : population) {
        if (agent.isActionLights(*zone)) {
            numberOfActiveAgents++;
            double power = agent.getPower();
            if (agent.getDesiredLightState(*zone)) {
                open = open + power;
            } else {
                close = close + power;
            }
        }
    }
    if (numberOfActiveAgents > 0.0) {
        if (open < close) {
            zone->setLightState(false);
        } else if (open > close) {
            zone->setLightState(true);
        } else {
            zone->setLightState(Utility::tossACoin());
        }
    }
}

void Building::setAgentCountForZone(std::shared_ptr<Building_Zone> zone) {
    double numberOfAgents = 0;
    for (Agent &agent : population) {
        if (agent.currentlyInZone(*zone)) {
            numberOfAgents++;
        }
    }
    zone->setOccupantFraction(numberOfAgents / population.size());
}

void Building::postTimeStep() {
  for (Agent &agent : population) {
      agent.postTimeStep();
  }
}

void Building::postprocess() {
    for (Agent &agent : population) {
        agent.postprocess();
    }
}

bool Building::hasZone(const std::string& zoneName) const {
  bool has = false;
  for (const std::shared_ptr<Building_Zone> & z : zones) {
    if (zoneName == z->getName()) {
      has = true;
    }
  }
  return has;
}
