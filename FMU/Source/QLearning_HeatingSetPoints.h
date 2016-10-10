// Copyright 2015 Jacob Chapman

#ifndef QLEARNING_HEATINGSETPOINTS_H_
#define QLEARNING_HEATINGSETPOINTS_H_

#include <string>
#include "QLearning.h"

class QLearning_HeatingSetPoints : public QLearning {
 public:
    QLearning_HeatingSetPoints();
    double learn();
    void reset();
    void setId(const int id);

 private:
    int reward_name;
    int action_name;
    int state_name;
    int previous_state_name;
};

#endif  // QLEARNING_HEATINGSETPOINTS_H_
