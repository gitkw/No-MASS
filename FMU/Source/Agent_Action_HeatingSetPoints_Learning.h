#ifndef AGENT_ACTION_HEATINGSETPOINTS_LEARNING_H
#define AGENT_ACTION_HEATINGSETPOINTS_LEARNING_H
#include <memory>
#include "QLearning_HeatingSetPoints.h"
#include "Agent_Action.h"

class Agent_Action_HeatingSetPoints_Learning : public Agent_Action
{
  public:
    Agent_Action_HeatingSetPoints_Learning();
    void step(const Building_Zone& zone, const bool inZone);
    void setup(const int id, const int learn);
    void print();
    void reset();
  private:
    QLearning_HeatingSetPoints qlWeekDay;
    QLearning_HeatingSetPoints qlWeekEnd;

    int learnId;
    int agentId;

    int previousHour;
    double setPoint;
    double steps;
    bool hasBeenInZone;
    double pmv;
    std::string pmv_name;
    std::string step_name;
  //  QLearning ql;
};

#endif // AGENT_ACTION_HEATINGSETPOINTS_LEARNING_H
