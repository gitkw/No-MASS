// Copyright 2015 Jacob Chapman

#include "DataStore.h"
#include "SimulationTime.h"
#include "QLearning_HeatingSetPoints.h"

void QLearning_HeatingSetPoints::setId(const int id) {
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

QLearning_HeatingSetPoints::QLearning_HeatingSetPoints() {
    actions = 20;
    action = 0;
    previous_state = 0;
}

void QLearning_HeatingSetPoints::reset() {}

double QLearning_HeatingSetPoints::learn() {
    int month = SimulationTime::month - 1;
    int hourOfDay = SimulationTime::hourOfDay;
    state = (month * 24) + hourOfDay;

    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);
    updateQ(previous_state, action, reward, state);
    previous_state = state;
    action = greedySelection(state);
    return action;
}
