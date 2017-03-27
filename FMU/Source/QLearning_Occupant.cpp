// Copyright 2015 Jacob Chapman

#include "DataStore.h"
#include "SimulationTime.h"
#include "QLearning_Occupant.h"

void QLearning_Occupant::setId(const int id) {
    this->id = id;
    reward_name =
      DataStore::addVariable(filename + "_reward" + std::to_string(id));
    action_name =
      DataStore::addVariable(filename + "_action" + std::to_string(id));
    state_name =
      DataStore::addVariable(filename + "_state" + std::to_string(id));
    previous_state_name =
      DataStore::addVariable(filename + "_previous_state" + std::to_string(id));
}

QLearning_Occupant::QLearning_Occupant() {}

void QLearning_Occupant::reset() {}

double QLearning_Occupant::learn() {
    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);
    updateQ(previous_state, action, reward, state);
    previous_state = state;
    action = greedySelection(state);
    return action;
}
