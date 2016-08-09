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

double Appliance::getPriority() const {
  return priority;
}

void Appliance::setPriority(double priority) {
  this->priority = priority;
}
