#ifndef QLEARNING_HEATINGSETPOINTS_H
#define QLEARNING_HEATINGSETPOINTS_H

#include <vector>
#include "QLearning.h"

class QLearning_HeatingSetPoints : public QLearning
{
    public:
      QLearning_HeatingSetPoints();

      double learn(const Building_Zone &zone);
      void reset();
      void setHeatingSetPoint(const double heatingSetPoint);
      void setId(const int id);

    private:

      double heatingSetPoint;
      std::string reward_name;
      std::string action_name;
      std::string state_name;
      std::string previous_state_name;
      std::string temp_name;
      std::string pmv_name;

};

#endif // QLEARNING_HEATINGSETPOINTS_H
