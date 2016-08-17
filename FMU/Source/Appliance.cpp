// Copyright 2016 Jacob Chapman

#include "Appliance.h"

Appliance::Appliance() {
}

double Appliance::powerAt(const int timestep) const {
  return power[timestep];
}

double Appliance::supplyAt(const int timestep) const {
  return supply[timestep];
}

double Appliance::powerBack() const {
  return power.back();
}

double Appliance::supplyBack() const {
  return supply.back();
}

double Appliance::getPriority() const {
  return priority;
}

void Appliance::setPriority(double priority) {
  this->priority = priority;
}

double Appliance::getCost() const {
  return cost;
}

void Appliance::setCost(double cost) {
  this->cost = cost;
}
