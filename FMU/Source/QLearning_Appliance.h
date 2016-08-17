// Copyright 2015 Jacob Chapman
#ifndef QLEARNING_APPLIANCE_H_
#define QLEARNING_APPLIANCE_H_

#include <string>
#include "QLearning.h"

class QLearning_Appliance : public QLearning {
 public:
  QLearning_Appliance();
  double learn();
  double getAction();
  void reset();
  void setId(const int id);

 private:
  std::string reward_name;
  std::string action_name;
  std::string state_name;
  std::string previous_state_name;
};

#endif  // QLEARNING_APPLIANCE_H_
