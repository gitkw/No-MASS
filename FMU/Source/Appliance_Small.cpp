// Copyright 2016 Jacob Chapman

#include <string>
#include <algorithm>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Small.h"

Appliance_Small::Appliance_Small() {}

void Appliance_Small::setup(applianceStruct a) {
  setID(a.id);
  setHoulyPriority(a.priority);
  model.setFolderLocation(SimulationConfig::FolderSmallAppliance);
  model.readWeibullParameters(a.WeibullParameters);
  model.readStateProbabilities(a.StateProbabilities);
  model.readFractions(a.Fractions);
  model.readSumRatedPowers(a.SumRatedPowers);
  model.setRatedPowerAt(Utility::randomInt(0, 24));
}

void Appliance_Small::step() {
  int stepcount = SimulationConfig::getStepCount();
  setPower(model.consumption(stepcount));
}
