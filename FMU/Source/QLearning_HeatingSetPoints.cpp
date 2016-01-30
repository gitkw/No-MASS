// Copyright 2015 Jacob Chapman
#include <iostream>
#include "DataStore.h"
#include "QLearning_HeatingSetPoints.h"

QLearning_HeatingSetPoints::QLearning_HeatingSetPoints() {
    states = 24 * 12;  // 40 * 24;
    actions = 40;
    action = 21;
    previous_state = 0;
}

double QLearning_HeatingSetPoints::learn(const Zone &zone) {
  double indoorTemp = zone.getMeanAirTemperature();
  int hourOfDay =
    DataStore::getValue("hourOfDay");
    int month =
      DataStore::getValue("month") - 1;

  // state = (hourOfDay * 40) + indoorTemp;
  state = month * 24 + hourOfDay;
  /*hourOfDay"
  std::cout << "state: " << state << std::endl;
  std::cout << "month: " << month << std::endl;
  std::cout << "hourOfDay: " << hourOfDay << std::endl;
  std::cout << "state: " << state << std::endl;
  std::cout << "hourOfDay: " << hourOfDay << std::endl;
  std::cout << "indoorTemp: " << indoorTemp << std::endl;
  std::cout << "****************" << std::endl;
  std::cout << "state: " << state << std::endl;
  std::cout << "action: " << action << std::endl;
  std::cout << "reward: " << reward << std::endl;
  std::cout << "previous_state: " << previous_state << std::endl;
  */
  updateQ(previous_state, action, reward, state);
  previous_state = state;
  action = greedySelection(state);
  // std::cout << "action: " << action << std::endl;
  return action;
}
