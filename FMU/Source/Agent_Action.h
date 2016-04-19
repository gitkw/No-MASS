#ifndef AGENT_ACTION_H
#define AGENT_ACTION_H

#include "Building_Zone.h"

class Agent_Action {
  public:
    Agent_Action();
    virtual void step(const Building_Zone& zone, const bool inZone,
                      const bool previouslyInZone,
                      const std::vector<double> &activities) = 0;

    virtual std::string getName() const;
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
    std::string name;
    int lengthOfTimeStepSeconds;
    std::vector<int> availableActivities;
};
#endif // AGENT_ACTION_H
