// Copyright 2015 Jacob Chapman

#include <iostream>
#include <string>
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

std::string State::getLocation() const {
     return zone->getName();
}

std::string State::getActivity() const {
    return activity;
}

void State::addState(State s) {
  states.push_back(s);
}

bool State::hasState(const int stateID) const {
  bool found = false;
  for (State s : states) {
      if (s.getId() == stateID || s.hasState(stateID)) {
          found = true;
        break;
      }
  }
  return found;
}

State State::getState(const int stateID) const {
  State x;
  for (State s : states) {
    if (s.getId() == stateID) {
      x = s;
      break;
    } else if (s.hasState(stateID)) {
      x = s.getState(stateID);
      break;
    }
  }
  return x;
}

Building_Zone* State::getZonePtr() const {
    return zone;
}
void State::setZonePtr(Building_Zone* zoneptr) {
    zone = zoneptr;
}
