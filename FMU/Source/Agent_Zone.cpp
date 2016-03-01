// Copyright 2016 Jacob Chapman

#include <iostream>
#include <vector>
#include <algorithm>

#include "Agent_Zone.h"

Agent_Zone::Agent_Zone() {
}

Agent_Zone::Agent_Zone(int id, const agentStruct &agent):id(id) {
  aahg.setup(id);
  availableActions.push_back(0);

  if (SimulationConfig::info.windows) {
      aaw.setup(agent.windowId);
      aaw.setOpenDuringCooking(agent.WindowOpenDuringCooking);
      aaw.setOpenDuringWashing(agent.WindowOpenDuringWashing);
      availableActions.push_back(1);
  }
  if (SimulationConfig::info.lights) {
      availableActions.push_back(3);
      aal.setOffDuringAudioVisual(agent.LightOffDuringAudioVisual);
      aal.setOffDuringSleep(agent.LightOffDuringSleep);
  }
  if (SimulationConfig::info.shading) {
      aas.setup(agent.shadeId);
      aas.setClosedDuringSleep(agent.ShadeClosedDuringSleep);
      aas.setClosedDuringWashing(agent.ShadeClosedDuringWashing);
      availableActions.push_back(2);
  }
}

void Agent_Zone::step(const Building_Zone& zone,
      const Building_Zone& zonePrevious,
      const std::vector<double> &activities) {
    if (id > 0) {
      bool inZone = zone.getId() == id;
      bool previouslyInZone = zonePrevious.getId() == id;

      if (inZone || previouslyInZone) {
        std::random_shuffle(availableActions.begin(), availableActions.end() );
        for (int a : availableActions) {
          if (inZone) {
            actionStep(a, zone, inZone, previouslyInZone, activities);
          }else if (previouslyInZone) {
            actionStep(a, zonePrevious, inZone, previouslyInZone, activities);
          }
        }
      }
    }
}

void Agent_Zone::actionStep(int action,
  const Building_Zone &zone, bool inZone, bool preZone,
  const std::vector<double> &activities) {
    switch (action) {
      case 0:
            if (inZone) {
              aahg.prestep(clo, metabolicRate);
              aahg.step(zone, inZone, preZone, activities);
              heatgains = aahg.getResult();
              previous_pmv = pmv;
              pmv = aahg.getPMV();
            }

        break;
      case 1:
            aaw.step(zone, inZone, preZone, activities);
            desiredWindowState = aaw.getResult();

        break;
      case 2:
            aas.step(zone, inZone, preZone, activities);
            desiredShadeState = aas.getResult();
        break;
      case 3:
            aal.step(zone, inZone, preZone, activities);
            desiredLightState = aal.getResult();
        break;
      case 4:
          // aah.step(zone, inZone, preZone, activities);
          //  heatState = aah.getResult();
        break;
      case 5:

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

double Agent_Zone::getDesiredShadeState() const {
  return desiredShadeState;
}

void Agent_Zone::setClo(double clo) {
  this->clo = clo;
}

void Agent_Zone::setMetabolicRate(double metabolicRate) {
  this->metabolicRate = metabolicRate;
}
