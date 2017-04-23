// Copyright 2015 Jacob Chapman

#include <iostream>
#include <string>
#include "Configuration.hpp"
#include "State.hpp"

State::State() {}
State::~State() {}

State::State(int id, double metabolicRate, double clo,
                  const std::string & activity) :
                  id(id), metabolicRate(metabolicRate),
                  clo(clo), activity(activity) {
}

unsigned int State::numberOfSubStates() const {
  return states.size();
}

void State::setId(int id){
  this->id = id;
}

void State::setMetabolicRate(double metabolicRate) {
    this->metabolicRate = metabolicRate;
}

void State::setClo(double clo) {
    this->clo = clo;
}

void State::setActivity(const std::string & activity) {
    this->activity = activity;
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
  states.push_back(s);
}

bool State::hasState(const int stateID) const {
  bool found = false;
  for (const State & s : states) {
      if (s.getId() == stateID || s.hasState(stateID)) {
          found = true;
        break;
      }
  }
  return found;
}

State State::getState(const int stateID) const {
  State x;
  for (const State & s : states) {
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

std::shared_ptr<Building_Zone> State::getZonePtr() const {
    return zone;
}
void State::setZonePtr(std::shared_ptr<Building_Zone> zoneptr) {
    zone = zoneptr;
}
