#ifndef Occupant_ACTION_H
#define Occupant_ACTION_H

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
#endif // Occupant_ACTION_H
