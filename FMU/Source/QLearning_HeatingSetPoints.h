#ifndef QLEARNING_HEATINGSETPOINTS_H
#define QLEARNING_HEATINGSETPOINTS_H

#include "Agent.h"
#include "QLearning.h"

class QLearning_HeatingSetPoints : public QLearning
{
    public:
      QLearning_HeatingSetPoints();

      double learn(const Zone &zone);
      void reset();

      void setId(int id);
    protected:
    private:
      int previousHour;
      double previousReward;
      std::string reward_name;
      std::string action_name;
      std::string state_name;
      std::string previous_state_name;

};

#endif // QLEARNING_HEATINGSETPOINTS_H
