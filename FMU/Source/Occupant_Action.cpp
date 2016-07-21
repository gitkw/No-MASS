// Copyright 2016 Jacob Chapman

#include <string>
#include <vector>
#include <algorithm>
#include "SimulationConfig.h"
#include "Occupant_Action.h"

Occupant_Action::Occupant_Action() {}

double Occupant_Action::getResult() const {
    return result;
}

void Occupant_Action::setAvailableActivities(
    const std::vector<int> availableActivities) {
    this->availableActivities = availableActivities;
}

bool Occupant_Action::activityAvailable(const int act) const {
  return std::find(availableActivities.begin(), availableActivities.end(), act)
      != availableActivities.end();
}

double Occupant_Action::getFutureDurationOfAbsenceState(
  const std::vector<double> &activities) const {
    int stepCount = SimulationConfig::getStepCount();
    unsigned int stepCounter = stepCount;
    double cdp = 0;
    if (!activityAvailable(activities.at(stepCount))) {
      double lengthOfTimeStepSeconds =
        (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      cdp = lengthOfTimeStepSeconds;
      while (stepCounter + 1 < activities.size()
          && !activityAvailable(activities.at(stepCounter+1))) {
          cdp = cdp + lengthOfTimeStepSeconds;
          stepCounter++;
      }
    }
    return cdp;
}

double Occupant_Action::getPreviousDurationOfAbsenceState(
    const std::vector<double> &activities) const {
  double cdp = 0;
  int stepCount = SimulationConfig::getStepCount();
  int stepCounter = stepCount - 1;
  double lengthOfTimeStepSeconds =
    (60 * (60 / SimulationConfig::info.timeStepsPerHour));
  while (stepCounter >= 0 && !activityAvailable(activities.at(stepCounter))) {
    cdp = cdp + lengthOfTimeStepSeconds;
    stepCounter--;
  }
  return cdp;
}

double Occupant_Action::getCurrentDurationOfPresenceState(
    const std::vector<double> &activities) const {
  double cdp = 0;
  int stepCount = SimulationConfig::getStepCount();
  int stepCounter = stepCount;
  if (activityAvailable(activities.at(stepCount))) {
    double lengthOfTimeStepSeconds =
      (60 * (60 / SimulationConfig::info.timeStepsPerHour));
    while (stepCounter > 0 && activityAvailable(activities.at(stepCounter-1))) {
      cdp = cdp + lengthOfTimeStepSeconds;
      stepCounter--;
    }
  }
  return cdp;
}


void Occupant_Action::setReward(const double reward) {
  this->reward = reward;
}

void Occupant_Action::setZoneId(const double zoneId) {
  this->zoneId = zoneId;
}
