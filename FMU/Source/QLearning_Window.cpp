// Copyright 2015 Jacob Chapman

#include "DataStore.h"
#include "QLearning_Window.h"

void QLearning_Window::setId(const int id) {
  this->id = id;
  reward_name =
    DataStore::addVariable(filename + "_reward"+ std::to_string(id));
  action_name =
    DataStore::addVariable(filename + "_action"+ std::to_string(id));
  state_name =
    DataStore::addVariable(filename + "_state"+ std::to_string(id));
  previous_state_name =
    DataStore::addVariable(filename + "_previous_state"+ std::to_string(id));
}

QLearning_Window::QLearning_Window() {
    actions = 2;
    action = 0;
    previous_state = 0;
}

void QLearning_Window::reset() {}

double QLearning_Window::learn() {
    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);
    updateQ(previous_state, action, reward, state);
    previous_state = state;
    action = greedySelection(state);
    return action;
}
