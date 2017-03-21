// Copyright 2015 Jacob Chapman

#ifndef QLEARNING_OCCUPANT_H_
#define QLEARNING_OCCUPANT_H_

#include <string>
#include "QLearning.h"

class QLearning_Occupant : public QLearning {
 public:
    QLearning_Occupant();
    double learn();
    void reset();
    void setId(const int id);

 private:
    int reward_name;
    int action_name;
    int state_name;
    int previous_state_name;
};

#endif  // QLEARNING_OCCUPANT_H_
