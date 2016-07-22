// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Large.h"

Appliance_Large::Appliance_Large() {
}

void Appliance_Large::setup() {
  model.setID(id);
  model.parseConfiguration("ApplianceUsage.xml");
}

void Appliance_Large::preprocess() {
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
  int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;

  for (int timeStep = 0; timeStep < totoaltimesteps; timeStep++) {
    double p = model.consumption(timeStep) *
                              (1 / SimulationConfig::info.timeStepsPerHour);
    // watt hour per timeperhour
    power.push_back(p);
  }
}
