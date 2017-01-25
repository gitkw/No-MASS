// Copyright 2015 Jacob Chapman

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include "SimulationTime.h"
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Occupant.h"
#include "Utility.h"
#include "Model_Presence.h"
#include "Building.h"


Building::Building() {}

void Building::setup(const buildingStruct &b) {
    name = b.name;
    id = b.id;
    std::string buildingID = "Building" + std::to_string(id) + "_";
    for (std::pair<std::string, ZoneStruct> z : b.zones) {
      zones.push_back(std::make_shared<Building_Zone>(Building_Zone()));
      zones.back()->setName(z.second.name);
      zones.back()->setActive(z.second.active);
      zones.back()->setIDString(buildingID + z.second.name);
      zones.back()->setup(z.second);
    }
    int popSize = b.agents.size();
    std::vector<int> pop = Utility::randomIntVect(popSize);
    // setup each agent randomly
    for (int a : pop) {
      std::string occid = std::to_string(a);
      population.push_back(Occupant());
      population.back().setBuildingID(id);
      population.back().setBuildingName(name);
      population.back().setIDString(buildingID + "Occupant" + occid + "_");
      population.back().setup(a, b.agents[a], zones);
    }
    appliances.setup(b);
}


void Building::preprocess() {
  appliances.preprocess();
}

void Building::setZones(
        const std::vector<std::shared_ptr<Building_Zone>> & zones) {
    this->zones = zones;
}


void Building::stepAppliancesUse() {
  appliances.stepLocal();
  appliances.stepLocalNegotiation();
}


void Building::stepAppliancesNegotiation(
                          const Contract_Negotiation & building_negotiation) {
      appliances.stepGlobalNegotiation(building_negotiation);
}

double Building::getPower() const {
  return appliances.getTotalPower();
}

void Building::step() {
    int popSize = population.size();
    std::vector<int> pop = Utility::randomIntVect(popSize);
    for (int a : pop) {
        population[a].step();
        appliances.addCurrentStates(population[a].getStateID());
    }

    for (std::shared_ptr<Building_Zone> &zone : zones) {
        if (!zone->isActive()) {
            continue;
        }
        setOccupantCountForZone(zone);

        if (SimulationConfig::info.windows
          || SimulationConfig::info.windowsLearn) {
            setOccupantWindowDecisionForZone(zone);
        }
        if (SimulationConfig::info.shading) {
            setOccupantBlindDecisionForZone(zone);
        }
        if (SimulationConfig::info.lights) {
            setOccupantLightDecisionForZone(zone);
        }
        setOccupantHeatDecisionsForZone(zone);
        setOccupantGainsForZone(zone);
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
    int hourOfDay = SimulationTime::hourOfDay;
    if (hourOfDay > 19 || hourOfDay < 6) {
      for (std::shared_ptr<Building_Zone> &zone : zones) {
        if (zone->isActive()) {
          zone->setBlindState(0);
        }
      }
    }
  }
}

double Building::decisionDoubleVec( const std::vector<double> & val,
                                    const std::vector<double> & power,
                                    const double currentState) const {
  double totalIncrease = 0;
  double totalDecrease = 0;
  double increase = 0;
  double decrease = 0;
  double same = 0;
  double increasePower = 0;
  double decreasePower = 0;
  double samePower = 0;
  for (unsigned int i = 0; i < val.size(); i++) {
    double d = val[i];
    double powerValue = power[i];

    if (d < currentState) {
      decrease++;
      totalDecrease = totalDecrease + d;
      decreasePower = decreasePower + powerValue;

    } else if (d > currentState) {
      increase++;
      totalIncrease = totalIncrease + d;
      increasePower = increasePower + powerValue;
    } else {
      same++;
      samePower = samePower + powerValue;
    }
  }
  double state = currentState;
  if ((samePower == increasePower) == decreasePower) {
      double i = Utility::randomInt(0, 2);
      if (i == 0) {
          state = totalIncrease / increase;
      } else if (i == 1) {
          state = totalDecrease / decrease;
      }
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
  }
  return state;
}

void Building::setOccupantHeatDecisionsForZone(
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

  for (Occupant &agent : population) {
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

void Building::setOccupantGainsForZone(std::shared_ptr<Building_Zone> zone) {
    double numberOfOccupants = 0;
    double totalRadientGains = 0;
    double aveRadientGains = 0;

    for (Occupant &agent : population) {
        if  (agent.isActionHeatGains(*zone)) {
            numberOfOccupants++;
            totalRadientGains += agent.getCurrentRadientGains(*zone);
        }
    }
    if (totalRadientGains > 0) {
        aveRadientGains = totalRadientGains / numberOfOccupants;
    }
    zone->setCurrentOccupantGains(aveRadientGains);
}

void Building::setAppGainsForZone(std::shared_ptr<Building_Zone> zone) {
    double totalRadientGainsAppliance = 0;
    double aveRadientGains = 0;
    double numberOfOccupantsAppliance = 0;

    for (Occupant &agent : population) {
        numberOfOccupantsAppliance++;
        if  (agent.isActionAppliance(*zone)) {
            totalRadientGainsAppliance += agent.getDesiredAppliance(*zone);
        }
    }
    if (totalRadientGainsAppliance > 0) {
        aveRadientGains += totalRadientGainsAppliance /
                                    numberOfOccupantsAppliance;
    }
    zone->setAppFraction(aveRadientGains);
}

void Building::setOccupantWindowDecisionForZone(
        std::shared_ptr<Building_Zone> zone) {
    double open = 0;
    double close = 0;
    int numberOfActiveOccupants = 0;

    for (const Occupant &agent : population) {
        if (agent.isActionWindow(*zone)) {
            double power = agent.getPower();
            numberOfActiveOccupants++;
            if (agent.getDesiredWindowState(*zone)) {
                open = open + power;
            } else {
                close = close + power;
            }
        }
    }
    if (numberOfActiveOccupants > 0) {
        zone->setWindowState(decisionBoolean(close, open));
    }
}

void Building::setOccupantBlindDecisionForZone(
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

    for (const Occupant &agent : population) {
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

bool Building::decisionBoolean(const double val1, const double val2) const {
  bool ret = val1 < val2;
  if (val1 == val2) {
      ret = Utility::tossACoin();
  }
  return ret;
}

void Building::setOccupantLightDecisionForZone(
        std::shared_ptr<Building_Zone> zone) {
    double open = 0;
    double close = 0;
    double numberOfActiveOccupants = 0;
    for (const Occupant &agent : population) {
        if (agent.isActionLights(*zone)) {
            numberOfActiveOccupants++;
            double power = agent.getPower();
            if (agent.getDesiredLightState(*zone)) {
                open = open + power;
            } else {
                close = close + power;
            }
        }
    }
    if (numberOfActiveOccupants > 0.0) {
        zone->setLightState(decisionBoolean(close, open));
    }
}

void Building::setOccupantCountForZone(std::shared_ptr<Building_Zone> zone) {
    double numberOfOccupants = 0;
    for (const Occupant &agent : population) {
        if (agent.currentlyInZone(*zone)) {
            numberOfOccupants++;
        }
    }
    zone->setOccupantFraction(numberOfOccupants);
}

void Building::postTimeStep() {
  for (Occupant &agent : population) {
      agent.postTimeStep();
  }
  appliances.postTimeStep();
}

void Building::postprocess() {
    for (Occupant &agent : population) {
        agent.postprocess();
    }
    appliances.postprocess();
}

bool Building::hasZone(const std::string& zoneName) const {
  bool has = false;
  for (const std::shared_ptr<Building_Zone> & z : zones) {
    if (z->isNamed(zoneName)) {
      has = true;
    }
  }
  return has;
}

int Building::getID() const {
    return id;
}

void Building::addContactsTo(Contract_Negotiation * building_negotiation) {
  appliances.addContactsTo(building_negotiation);
}
