// Copyright 2015 Jacob Chapman

#include "DataStore.h"
#include "QLearning_Appliance.h"

void QLearning_Appliance::setId(const int id) {
    this->id = id;
    reward_name = filename + "_reward"+ std::to_string(id);
    action_name = filename + "_action"+ std::to_string(id);
    state_name = filename + "_state"+ std::to_string(id);
    previous_state_name = filename + "_previous_state"+ std::to_string(id);
    DataStore::addVariable(reward_name);
    DataStore::addVariable(action_name);
    DataStore::addVariable(state_name);
    DataStore::addVariable(previous_state_name);
}

QLearning_Appliance::QLearning_Appliance() {
    actions = 24;
    action = 0;
    previous_state = 0;
}

void QLearning_Appliance::reset() {}

double QLearning_Appliance::learn() {
    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);
    updateQ(previous_state, action, reward, state);
    return action;
}

double QLearning_Appliance::getAction() {
  action = greedySelection(state);
  previous_state = state;
    return action;
}
