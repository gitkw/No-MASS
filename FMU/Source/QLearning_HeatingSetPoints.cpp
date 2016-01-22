// Copyright 2015 Jacob Chapman
#include <iostream>
#include "QLearning_HeatingSetPoints.h"

QLearning_HeatingSetPoints::QLearning_HeatingSetPoints() {
    states = 40;
    actions = 40;
    action = 0;
    previous_state = 0;
}

double QLearning_HeatingSetPoints::learn(const Zone &zone) {
      double indoorTemp = zone.getMeanAirTemperature();
      state = indoorTemp;
      /*
      std::cout << "****************" << std::endl;
      std::cout << "state: " << state << std::endl;
      std::cout << "action: " << action << std::endl;
      std::cout << "reward: " << reward << std::endl;
      std::cout << "previous_state: " << previous_state << std::endl;
      */
      updateQ(previous_state, action, reward, state);
      previous_state = state;
      action = greedySelection(state);
      return action;
}
