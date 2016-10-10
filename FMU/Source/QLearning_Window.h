// Copyright 2015 Jacob Chapman

#ifndef QLEARNING_WINDOW_H
#define QLEARNING_WINDOW_H

#include <string>
#include "QLearning.h"

class QLearning_Window : public QLearning {
 public:
    QLearning_Window();

    double learn();
    void reset();
    void setId(const int id);

 private:
    int reward_name;
    int action_name;
    int state_name;
    int previous_state_name;
};

#endif  // QLEARNING_WINDOW_H_
