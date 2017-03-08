// Copyright 2015 Jacob Chapman

#include <iostream>
#include <string>
#include <utility>
#include "SimulationConfig.h"
#include "State.h"

State::State() {}
State::~State() {}

unsigned int State::numberOfSubStates() const {
  return states.size();
}

int State::getId() const {
    return id;
}
double State::getMetabolicRate()const {
    return metabolicRate;
}
double State::getClo()const {
    return clo;
}

bool State::isInActivity(const std::string & activity) const {
  return this->activity == activity;
}

void State::addState(State s) {
  states.insert({s.getId(), s});
}

bool State::hasState(const int stateID) const {
  bool found = states.end() != states.find(stateID) ;
  if (!found) {
    for (const auto & s : states) {
      found = s.second.hasState(stateID);
      if (found) {
        break;
      }
    }
  }
  return found;
}

State State::getState(const int stateID) const {
  std::unordered_map<int, State>::const_iterator si = states.find(stateID) ;
  State x;
  if (states.end() == si && hasState(stateID)) {
    for (const auto & s : states) {
      if (s.second.hasState(stateID)) {
        x = s.second.getState(stateID);
        break;
      }
    }
  }else{
    x = states.at(stateID);
  }
  return x;
}

std::shared_ptr<Building_Zone> State::getZonePtr() const {
    return zone;
}
void State::setZonePtr(std::shared_ptr<Building_Zone> zoneptr) {
    zone = zoneptr;
}
