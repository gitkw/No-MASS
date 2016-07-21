// Copyright 2016 Jacob Chapman

#include <vector>
#include "Utility.h"
#include "Occupant_Action_Appliance_BDI.h"

Occupant_Action_Appliance_BDI::Occupant_Action_Appliance_BDI() {
  ApplianceDuringDay = 0.0;
}

void Occupant_Action_Appliance_BDI::setApplianceDuringDay(
                                      double ApplianceDuringDay) {
  this->ApplianceDuringDay = ApplianceDuringDay;
}

bool Occupant_Action_Appliance_BDI::doRecipe(
                            const std::vector<double> &activities) {
  bool bdi = false;
  if (ApplianceDuringDay > 0) {
    bdi = true;
    result = 0;
    int stepCount = SimulationConfig::getStepCount();
    if (stepCount == 0) {
      first = -1;
      last = -1;
    }
    if (last < stepCount) {
      calculateFirstLastCouts(activities);
    }
    if (ApplianceDuringDay > Utility::randomDouble(0, 1)) {
      if (stepCount >= first && stepCount <= last) {
        result = 1;
      }
    }
  }
  return bdi;
}

void Occupant_Action_Appliance_BDI::calculateFirstLastCouts(
      const std::vector<double> &activities) {
  int stepCount = SimulationConfig::getStepCount();
  if (stepCount > first) {
    int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
    int day = hour / 24;
    int dayCounter = day;
    int counter = stepCount;

    while (stepCount > first) {
      if (activities[counter] < 9) {
        first = counter;
        break;
      }
      counter++;
    }
    while (day == dayCounter) {
      if (activities[counter] < 9) {
        last = counter;
      }
      hour = (counter * SimulationConfig::lengthOfTimestep()) / 3600;
      dayCounter = hour / 24;
      counter++;
    }
  }
}
