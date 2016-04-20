// Copyright 2016 Jacob Chapman

#include <string>
#include <vector>
#include <algorithm>
#include "SimulationConfig.h"
#include "Agent_Action.h"

Agent_Action::Agent_Action() {}

double Agent_Action::getResult() const {
    return result;
}

void Agent_Action::setAvailableActivities(
    const std::vector<int> availableActivities) {
    this->availableActivities = availableActivities;
}

bool Agent_Action::activityAvailable(const int act) const {
  return std::find(availableActivities.begin(), availableActivities.end(), act)
      != availableActivities.end();
}

double Agent_Action::getFutureDurationOfPresenceState(
  const std::vector<double> &activities) const {
    int stepCount = SimulationConfig::getStepCount();
    unsigned int stepCounter = stepCount;
    double cdp = 0;
    if (activityAvailable(activities.at(stepCount))) {
      double lengthOfTimeStepSeconds =
        (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      cdp = lengthOfTimeStepSeconds;
      while (stepCounter + 1 < activities.size()
          && activityAvailable(activities.at(stepCounter+1))) {
          cdp = cdp + lengthOfTimeStepSeconds;
          stepCounter++;
      }
    }
    return cdp;
}

double Agent_Action::getPreviousDurationOfAbsenceState(
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

double Agent_Action::getCurrentDurationOfPresenceState(
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
