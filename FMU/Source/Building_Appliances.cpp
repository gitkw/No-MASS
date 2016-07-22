// Copyright 2016 Jacob Chapman

#include <string>
#include <list>
#include <vector>

#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building_Appliances.h"

Building_Appliances::Building_Appliances() {}

void Building_Appliances::setup() {
  std::string buildApp = "Appliance" + std::to_string(buildingID) + "_";
  DataStore::addVariable(buildApp + "sum");
  DataStore::addVariable(buildApp + "sum_small");
  DataStore::addVariable(buildApp + "sum_large");
  std::vector<int> app =
                  SimulationConfig::buildings[buildingID].AppliancesLarge;
  for (const int i : app) {
    large.push_back(Appliance_Large());
    large.back().setID(i);
    large.back().setBuildingID(buildingID);
    large.back().setup();
    DataStore::addVariable(buildApp + std::to_string(i));
  }

  std::vector<appSmallStruct> appSmall =
                  SimulationConfig::buildings[buildingID].AppliancesSmall;
  for (const appSmallStruct s : appSmall) {
    small.push_back(Appliance_Small());
    small.back().setWeibullParameters(s.WeibullParameters);
    small.back().setStateProbabilities(s.StateProbabilities);
    small.back().setFractions(s.Fractions);
    small.back().setSumRatedPowers(s.SumRatedPowers);
    small.back().setup();
  }
}

void Building_Appliances::preprocess() {
  std::list<int> pop = Utility::randomIntList(large.size());
  for (int a : pop) {
      large[a].preprocess();
  }
  std::list<int> pops = Utility::randomIntList(small.size());
  for (int a : pops) {
      small[a].preprocess();
  }
}

void Building_Appliances::step() {
  std::string buildApp = "Appliance" + std::to_string(buildingID) + "_";
  double sum = 0;
  std::list<int> pop = Utility::randomIntList(large.size());
  for (int a : pop) {
    int appid = large[a].getID();
    double power = large[a].powerAt(SimulationConfig::getStepCount());
    sum += power;
    DataStore::addValue(buildApp + std::to_string(appid), power);
  }
  DataStore::addValue(buildApp + "sum_large", sum);
  pop = Utility::randomIntList(small.size());
  double sum_small = 0;
  for (int a : pop) {
      double power = small[a].powerAt(SimulationConfig::getStepCount());
      sum += power;
      sum_small += power;
  }
  DataStore::addValue(buildApp + "sum_small", sum_small);
  DataStore::addValue(buildApp + "sum", sum);
}

void Building_Appliances::postprocess() {
}

void Building_Appliances::postTimeStep() {
}

void Building_Appliances::setBuildingID(const int id) {
    buildingID = id;
}
