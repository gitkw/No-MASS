// Copyright 2016 Jacob Chapman

#include <string>
#include <list>
#include <vector>
#include <cassert>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building_Appliances.h"

Building_Appliances::Building_Appliances() {}

void Building_Appliances::setup() {
  buildingString = "Building" +
                                  std::to_string(buildingID) + "_Appliance";
  DataStore::addVariable(buildingString + "sum");
  DataStore::addVariable(buildingString + "sum_small");
  DataStore::addVariable(buildingString + "sum_large");

  std::vector<appPVStruct> appPV =
                  SimulationConfig::buildings[buildingID].AppliancesPV;
  for (const appPVStruct s : appPV) {
    pv.push_back(Appliance_PV());
    pv.back().setID(s.id);
    pv.back().setPriority(s.priority);
    pv.back().setBuildingID(buildingID);
    pv.back().setFileName(s.file);
    pv.back().setup();
    DataStore::addVariable(buildingString + std::to_string(s.id) + "_supplied");
  }

  std::vector<appLargeStruct> app =
                  SimulationConfig::buildings[buildingID].AppliancesLarge;
  for (const appLargeStruct &s : app) {
    large.push_back(Appliance_Large());
    large.back().setID(s.id);
    large.back().setPriority(s.priority);
    large.back().setBuildingID(buildingID);
    large.back().setActivities(s.activities);

    large.back().setup();
    DataStore::addVariable(buildingString + std::to_string(s.id) + "_recieved");
    DataStore::addVariable(buildingString +
                                          std::to_string(s.id) + "_requested");
  }

  std::vector<appSmallStruct> appSmall =
                  SimulationConfig::buildings[buildingID].AppliancesSmall;
  for (const appSmallStruct s : appSmall) {
    small.push_back(Appliance_Small());
    small.back().setID(s.id);
    small.back().setPriority(s.priority);
    small.back().setWeibullParameters(s.WeibullParameters);
    small.back().setStateProbabilities(s.StateProbabilities);
    small.back().setFractions(s.Fractions);
    small.back().setSumRatedPowers(s.SumRatedPowers);
    small.back().setup();
    DataStore::addVariable(buildingString + std::to_string(s.id) + "_recieved");
    DataStore::addVariable(buildingString +
                                        std::to_string(s.id) + "_requested");
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
  std::list<int> popp = Utility::randomIntList(pv.size());
  for (int a : popp) {
      pv[a].preprocess();
  }
}

void Building_Appliances::sendContract(int id, double priority,
                                        double request, double supply) {
  contract c;
  c.id = id;
  c.requested = request;
  c.priority = priority;
  c.supplied = supply;
  app_negotiation.submit(c);
}

void Building_Appliances::stepLarge() {
  std::list<int> pop = Utility::randomIntList(large.size());
  int stepCount = SimulationConfig::getStepCount();
  for (int a : pop) {
    int appid = large[a].getID();
    large[a].hasActivities(currentStates);
    double priority = large[a].getPriority();
    double power = 0;
    double supply = 0;
    large[a].step();
    power = large[a].powerAt(stepCount);
    supply = large[a].supplyAt(stepCount);
    assert(power >= 0);
    sendContract(appid, priority, power, supply);
    DataStore::addValue(buildingString +
                                  std::to_string(appid) + "_requested", power);
  }
}

void Building_Appliances::step() {
  std::list<int> pop = Utility::randomIntList(pv.size());
  for (int a : pop) {
    int appid = pv[a].getID();
    double supply = pv[a].supplyAt(SimulationConfig::getStepCount());
    double priority = pv[a].getPriority();
    sendContract(appid, priority, 0, supply);
    DataStore::addValue(buildingString +
                                std::to_string(appid) + "_supplied", supply);
  }

  stepLarge();
  pop = Utility::randomIntList(small.size());
  for (int a : pop) {
    int appid = small[a].getID();
    double power = small[a].powerAt(SimulationConfig::getStepCount());
    double supply = small[a].supplyAt(SimulationConfig::getStepCount());
    double priority = small[a].getPriority();
    sendContract(appid, priority, power, supply);
    DataStore::addValue(buildingString +
                                std::to_string(appid) + "_requested", power);
  }
  app_negotiation.process();
  double sum_large = 0;
  pop = Utility::randomIntList(large.size());
  for (int a : pop) {
    int appid = large[a].getID();
    contract c = app_negotiation.getContract(appid);
    double power = c.recieved;
    sum_large += power;
    DataStore::addValue(buildingString +
                                  std::to_string(appid) + "_recieved", power);
  }

  double sum_small = 0;
  pop = Utility::randomIntList(small.size());
  for (int a : pop) {
    int appid = small[a].getID();
    contract c = app_negotiation.getContract(appid);
    double power = c.recieved;
    sum_small += power;
    DataStore::addValue(buildingString +
                                  std::to_string(appid) + "_recieved", power);
  }
  totalPower = sum_small + sum_large;
  DataStore::addValue(buildingString + "sum_small", sum_small);
  DataStore::addValue(buildingString + "sum_large", sum_large);
  DataStore::addValue(buildingString + "sum", totalPower);

  currentStates.clear();
  app_negotiation.clear();
}

void Building_Appliances::postprocess() {
}

void Building_Appliances::postTimeStep() {
}

void Building_Appliances::setBuildingID(const int id) {
    buildingID = id;
}

double Building_Appliances::getTotalPower() const {
  return totalPower;
}

void Building_Appliances::addCurrentStates(const int stateid) {
    currentStates.push_back(stateid);
}
