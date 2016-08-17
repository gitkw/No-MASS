// Copyright 2016 Jacob Chapman

#include <cassert>
#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Large.h"

Appliance_Large::Appliance_Large() {
}

void Appliance_Large::setup() {
  setupModel();
}

void Appliance_Large::setupModel() {
  model.setID(id);
  model.parseConfiguration(SimulationConfig::RunLocation +
                                                "AppliancesLarge.xml");
}

void Appliance_Large::step() {
  model.decreaseDuration();
  double p = 0.0;
  if (isOn() || match) {
    int stepCount = SimulationConfig::getStepCount();
    p = model.consumption(stepCount);
  }
  power.push_back(p);
  supply.push_back(0.0);
}

bool Appliance_Large::isOn() const {
  return model.isOn();
}

bool Appliance_Large::hasActivities(std::vector<int> Activities) {
  match = this->Activities.empty();
  for (int i : this->Activities) {
    for (int j : Activities) {
      if (i == j) {
          match = true;
          break;
      }
    }
  }
  return match;
}

void Appliance_Large::setActivities(const std::vector<int> Activities) {
  this->Activities = Activities;
}
