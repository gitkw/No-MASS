#ifndef AGENT_ACTION_LEARNING_H
#define AGENT_ACTION_LEARNING_H
#include <memory>
#include "QLearning.h"
#include "Agent_Action.h"

class Agent_Action_Learning : public Agent_Action
{
  public:
    Agent_Action_Learning();
    void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);
    void setup(const int id, const int learn);
    void setReward(double reward);
    void print();
    void reset();
  private:
    std::shared_ptr<QLearning> ql;
    double reward;
    int learnId;
  //  QLearning ql;
};

#endif // AGENT_ACTION_LEARNING_H
