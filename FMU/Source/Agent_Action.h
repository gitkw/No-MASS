#ifndef AGENT_ACTION_H
#define AGENT_ACTION_H

#include "Building_Zone.h"

class Agent_Action {
  public:
    Agent_Action();

    void setAvailableActivities(const std::vector<int> availableActivities);
    double getResult() const;
    double getFutureDurationOfPresenceState(
                      const std::vector<double> &activities) const;
    double getPreviousDurationOfAbsenceState(
                      const std::vector<double> &activities) const;
    double getCurrentDurationOfPresenceState(
                      const std::vector<double> &activities) const;

  protected:
    bool activityAvailable(const int act) const;
    double result;
    std::vector<int> availableActivities;
};
#endif // AGENT_ACTION_H
