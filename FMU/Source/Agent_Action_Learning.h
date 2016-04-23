#ifndef AGENT_ACTION_LEARNING_H
#define AGENT_ACTION_LEARNING_H
#include <memory>
#include "QLearning.h"
#include "Agent_Action.h"

class Agent_Action_Learning : public Agent_Action
{
  public:
    Agent_Action_Learning();
    void step(const Building_Zone& zone, const bool inZone);
    void setup(const int id, const int learn);
    void setReward(const double reward);
    void setZoneId(const double zoneId);
    void print();
    void reset();
  private:
    std::shared_ptr<QLearning> qlWeekDay;
    std::shared_ptr<QLearning> qlWeekEnd;
    double reward;
    int learnId;
    int agentId;
    int zoneId;

    int previousHour;
    double setPoint;
    double steps;
    bool hasBeenInZone;
    double pmv;
    std::string pmv_name;
    std::string step_name;
  //  QLearning ql;
};

#endif // AGENT_ACTION_LEARNING_H
