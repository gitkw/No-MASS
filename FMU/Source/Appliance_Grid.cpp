// Copyright 2016 Jacob Chapman

#include "Appliance_Grid.h"

Appliance_Grid::Appliance_Grid() {}

void Appliance_Grid::setRequiredPower(const double power) {
  this->requiredPower = power;
}

void Appliance_Grid::step() {
  if (requiredPower < 0) {
    requiredPower = 0;
  }
  power.push_back(0.0);
  supply.push_back(requiredPower);
}
