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
  int reward_name;
  int action_name;
  int state_name;
  int previous_state_name;
};

#endif  // QLEARNING_APPLIANCE_H_
