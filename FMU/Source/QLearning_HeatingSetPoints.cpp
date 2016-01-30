// Copyright 2015 Jacob Chapman
#include <iostream>
#include <string>
#include "DataStore.h"
#include "QLearning_HeatingSetPoints.h"




void QLearning_HeatingSetPoints::setId(int id) {
    this->id = id;

    reward_name = "reward"+ std::to_string(id);
    action_name = "action"+ std::to_string(id);
    state_name = "state"+ std::to_string(id);
    previous_state_name = "previous_state"+ std::to_string(id);
    DataStore::addVariable(reward_name);
    DataStore::addVariable(action_name);
    DataStore::addVariable(state_name);
    DataStore::addVariable(previous_state_name);

}


QLearning_HeatingSetPoints::QLearning_HeatingSetPoints() {
    states = 24 * 12;  // 40 * 24;
    actions = 30;
    action = 0;
    previous_state = 0;
    previousHour = -1;
}

void QLearning_HeatingSetPoints::reset() {
  previousHour = -1;
  previousReward = -1;
}

double QLearning_HeatingSetPoints::learn(const Zone &zone) {
  int hour = DataStore::getValue("hour");
  // std::cout << "hour: " << hour << " prevHour: " << previousHour << std::endl;
  // std::cout << "reward: " << reward << " prevReward: " << previousReward << std::endl;
  if ( hour != previousHour) {

  //   std::cout << "leanr: " << reward << std::endl;
    int month = DataStore::getValue("month") - 1;
    int hourOfDay = DataStore::getValue("hourOfDay");

    state = (month * 24) + hourOfDay;
    action = zone.getHeatingState();
    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);

    //updateQ(previous_state, action, reward, state);
    previous_state = state;
    previousReward = reward;
    action = greedySelection(state);

    previousHour = hour;
  }
  return action;
}
