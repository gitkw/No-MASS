// Copyright 2016 Jacob Chapman

#include <cassert>
#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Large_Learning.h"

Appliance_Large_Learning::Appliance_Large_Learning() {
}

void Appliance_Large_Learning::setup() {
}

void Appliance_Large_Learning::step() {
  power.push_back(0.0);
  supply.push_back(0.0);
}
