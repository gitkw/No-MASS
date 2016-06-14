// Copyright 2016 Jacob Chapman

#include <iostream>
#include <vector>
#include "Utility.h"
#include "Agent_Action_Appliance.h"

Agent_Action_Appliance::Agent_Action_Appliance() {
  ApplianceDuringDay = 0.0;
}

void Agent_Action_Appliance::setApplianceDuringDay(double ApplianceDuringDay) {
  this->ApplianceDuringDay = ApplianceDuringDay;
}

bool Agent_Action_Appliance::BDI(const std::vector<double> &activities) {
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

void Agent_Action_Appliance::calculateFirstLastCouts(
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
