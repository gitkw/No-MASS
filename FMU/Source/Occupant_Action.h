// Copyright 2016 Jacob Chapman

#ifndef OCCUPANT_ACTION_H_
#define OCCUPANT_ACTION_H_

#include <vector>
#include "Building_Zone.h"

class Occupant_Action {
 public:
  Occupant_Action();

  void setAvailableActivities(const std::vector<int> availableActivities);
  double getResult() const;
  double getFutureDurationOfAbsenceState(
                    const std::vector<double> &activities) const;
  double getPreviousDurationOfAbsenceState(
                    const std::vector<double> &activities) const;
  double getCurrentDurationOfPresenceState(
                    const std::vector<double> &activities) const;

  void setZoneId(const double zoneId);
  void setReward(const double reward);

 protected:
  bool activityAvailable(const int act) const;
  double result;
  double reward;
  std::vector<int> availableActivities;
  int zoneId;
};
#endif  // OCCUPANT_ACTION_H_
