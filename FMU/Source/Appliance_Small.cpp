// Copyright 2016 Jacob Chapman

#include <string>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Small.h"

Appliance_Small::Appliance_Small() {}

void Appliance_Small::setup() {
  model.setFolderLocation(SimulationConfig::RunLocation + "SmallAppliances/");
  model.readWeibullParameters(WeibullParameters);
  model.readStateProbabilities(StateProbabilities);
  model.readFractions(Fractions);
  model.readSumRatedPowers(SumRatedPowers);
  model.setRatedPowerAt(Utility::randomInt(0, 24));
}

void Appliance_Small::preprocess() {
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
  int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;

  for (int timeStep = 0; timeStep < totoaltimesteps; timeStep++) {
    power.push_back(model.consumption(timeStep));
    supply.push_back(0.0);
    supplyCost.push_back(0.0);
  }
}

void Appliance_Small::setWeibullParameters(
                                        const std::string & WeibullParameters) {
  this->WeibullParameters = WeibullParameters;
}
void Appliance_Small::setStateProbabilities(
                                      const std::string & StateProbabilities) {
  this->StateProbabilities = StateProbabilities;
}
void Appliance_Small::setFractions(const std::string & Fractions) {
  this->Fractions = Fractions;
}

void Appliance_Small::setSumRatedPowers(const std::string & SumRatedPowers) {
  this->SumRatedPowers = SumRatedPowers;
}
