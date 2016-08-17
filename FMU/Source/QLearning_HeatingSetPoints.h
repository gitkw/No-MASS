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
    std::string reward_name;
    std::string action_name;
    std::string state_name;
    std::string previous_state_name;
};

#endif  // QLEARNING_HEATINGSETPOINTS_H_
