// Copyright 2016 Jacob Chapman

#include <string>
#include <list>
#include <vector>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building_Appliances.h"

Building_Appliances::Building_Appliances() {
    PowerRequested = 0;
    PowerGenerated = 0;
}

void Building_Appliances::setup() {
  buildingString = "Building" + std::to_string(buildingID) + "_Appliance";
  DataStore::addVariable(buildingString + "_Sum_Recieved");
  DataStore::addVariable(buildingString + "_Sum_Small");
  DataStore::addVariable(buildingString + "_Sum_Large");
  DataStore::addVariable(buildingString + "_Sum_Cost");

  std::vector<appPVStruct> appPV =
                  SimulationConfig::buildings[buildingID].AppliancesPV;
  for (const appPVStruct s : appPV) {
    pv.push_back(Appliance_PV());
    pv.back().setID(s.id);
    pv.back().setPriority(s.priority);
    pv.back().setCost(s.cost);
    pv.back().setBuildingID(buildingID);
    pv.back().setFileName(s.file);
    pv.back().setup();
    addAppToDataStrore(s.id);
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
    addAppToDataStrore(s.id);
  }

  app = SimulationConfig::buildings[buildingID].AppliancesLargeLearning;
  for (const appLargeStruct &s : app) {
    largeLearning.push_back(Appliance_Large_Learning());
    largeLearning.back().setID(s.id);
    largeLearning.back().setPriority(s.priority);
    largeLearning.back().setBuildingID(buildingID);
    largeLearning.back().setActivities(s.activities);
    largeLearning.back().setup();
    addAppToDataStrore(s.id);
  }

  app = SimulationConfig::buildings[buildingID].AppliancesGrid;
  for (const appLargeStruct &s : app) {
    grid.push_back(Appliance_Grid());
    grid.back().setID(s.id);
    grid.back().setPriority(s.priority);
    grid.back().setCost(s.cost);
    grid.back().setBuildingID(buildingID);
    grid.back().setup();
    addAppToDataStrore(s.id);
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
    addAppToDataStrore(s.id);
  }

  std::vector<appFMIStruct> appFMI =
                  SimulationConfig::buildings[buildingID].AppliancesFMI;
  for (const appFMIStruct s : appFMI) {
    fmi.push_back(Appliance_FMI());
    fmi.back().setID(s.id);
    fmi.back().setPriority(s.priority);
    fmi.back().setFMIVariableName(s.variableName);
    fmi.back().setup();
    addAppToDataStrore(s.id);
  }
}

void Building_Appliances::preprocess() {
  std::list<int> pop = Utility::randomIntList(small.size());
  for (int a : pop) {
      small[a].preprocess();
  }
  pop = Utility::randomIntList(pv.size());
  for (int a : pop) {
      pv[a].preprocess();
  }
}

void Building_Appliances::sendContract(int id, double priority,
                                double request, double supply, double cost) {
  contract c;
  c.id = id;
  c.requested = request;
  c.priority = priority;
  c.supplied = supply;
  c.suppliedCost = cost;
  app_negotiation.submit(c);
  PowerRequested += request;
  PowerGenerated += supply;
  addAppVariableToDataStrore(id, request, supply);
}

void Building_Appliances::stepLarge() {
  std::list<int> pop = Utility::randomIntList(large.size());
  for (int a : pop) {
    large[a].hasActivities(currentStates);
    large[a].step();
    int appid = large[a].getID();
    double priority = large[a].getPriority();
    double power = large[a].powerBack();
    double supply = large[a].supplyBack();
    sendContract(appid, priority, power, supply, 0);
  }
}

void Building_Appliances::stepLargeLearning() {
  std::list<int> pop = Utility::randomIntList(largeLearning.size());
  for (int a : pop) {
    largeLearning[a].hasActivities(currentStates);
    largeLearning[a].step();
    int appid = largeLearning[a].getID();
    double priority = largeLearning[a].getPriority();
    double power = largeLearning[a].powerBack();
    double supply = largeLearning[a].supplyBack();
    sendContract(appid, priority, power, supply, 0);
  }
}

void Building_Appliances::stepSmall() {
  std::list<int> pop = Utility::randomIntList(small.size());
  for (int a : pop) {
    int appid = small[a].getID();
    double power = small[a].powerAt(SimulationConfig::getStepCount());
    double supply = small[a].supplyAt(SimulationConfig::getStepCount());
    double priority = small[a].getPriority();
    sendContract(appid, priority, power, supply, 0);
  }
}

void Building_Appliances::stepPV() {
  std::list<int> pop = Utility::randomIntList(pv.size());
  for (int a : pop) {
    int appid = pv[a].getID();
    double supply = pv[a].supplyAt(SimulationConfig::getStepCount());
    double priority = pv[a].getPriority();
    double cost = pv[a].getCost();
    sendContract(appid, priority, 0, supply, cost);
  }
}

void Building_Appliances::stepFMI() {
  std::list<int> pop = Utility::randomIntList(fmi.size());
  for (int a : pop) {
    fmi[a].step();
    int appid = fmi[a].getID();
    double power = fmi[a].powerBack();
    double supply = fmi[a].supplyBack();
    double priority = fmi[a].getPriority();
    sendContract(appid, priority, power, supply, 0);
  }
}

void Building_Appliances::stepGrid() {
  std::list<int> pop = Utility::randomIntList(grid.size());
  for (int a : pop) {
    grid[a].setRequiredPower(PowerRequested - PowerGenerated);
    grid[a].step();
    int appid = grid[a].getID();
    double priority = grid[a].getPriority();
    double power = grid[a].powerBack();
    double supply = grid[a].supplyBack();
    double cost = grid[a].getCost();
    sendContract(appid, priority, power, supply, cost);
  }
}

void Building_Appliances::step() {
  PowerRequested = 0;
  PowerGenerated = 0;
  stepPV();
  stepLarge();
  stepSmall();
  stepFMI();
  stepLargeLearning();
  stepGrid();  // Grid must step last

  app_negotiation.process();
  double sum_large = 0;
  double sum_cost = 0;
  std::list<int> pop = Utility::randomIntList(large.size());
  for (int a : pop) {
    int appid = large[a].getID();
    double power = app_negotiation.getRecievedPowerForContract(appid);
    double cost = app_negotiation.getCostOfPowerForContract(appid);
    sum_large += power;
    sum_cost += cost;
    addAppRecievedToDataStrore(appid, power, cost);
  }

  pop = Utility::randomIntList(largeLearning.size());
  for (int a : pop) {
    int appid = largeLearning[a].getID();
    double power = app_negotiation.getRecievedPowerForContract(appid);
    double cost = app_negotiation.getCostOfPowerForContract(appid);
    largeLearning[a].addToCost(cost);
    sum_large += power;
    sum_cost += cost;
    addAppRecievedToDataStrore(appid, power, cost);
  }

  double sum_small = 0;
  pop = Utility::randomIntList(small.size());
  for (int a : pop) {
    int appid = small[a].getID();
    double power = app_negotiation.getRecievedPowerForContract(appid);
    double cost = app_negotiation.getCostOfPowerForContract(appid);
    sum_small += power;
    sum_cost += cost;
    addAppRecievedToDataStrore(appid, power, cost);
  }

  double sum_fmi = 0;
  pop = Utility::randomIntList(fmi.size());
  for (int a : pop) {
    int appid = fmi[a].getID();
    double power = app_negotiation.getRecievedPowerForContract(appid);
    double cost = app_negotiation.getCostOfPowerForContract(appid);
    sum_fmi += power;
    sum_cost += cost;
    addAppRecievedToDataStrore(appid, power, cost);
  }
  double totalPowerRecieved = sum_small + sum_large + sum_fmi;
  DataStore::addValue(buildingString + "_Sum_Small", sum_small);
  DataStore::addValue(buildingString + "_Sum_Large", sum_large);
  DataStore::addValue(buildingString + "_Sum_fmi", sum_fmi);
  DataStore::addValue(buildingString + "_Sum_Recieved", totalPowerRecieved);
  DataStore::addValue(buildingString + "_Sum_Cost", sum_cost);
  totalPower = PowerRequested - PowerGenerated;
  currentStates.clear();
  app_negotiation.clear();
}

void Building_Appliances::postprocess() {
  std::list<int> pop = Utility::randomIntList(largeLearning.size());
  for (int a : pop) {
    largeLearning[a].postprocess();
  }
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

void Building_Appliances::addAppVariableToDataStrore(const int id,
                                const int requested, const int supplied) {
  std::string s_id = buildingString + std::to_string(id);
  DataStore::addValue(s_id + "_supplied", supplied);
  DataStore::addValue(s_id + "_requested", requested);
}

void Building_Appliances::addAppRecievedToDataStrore(const int id,
                                  const int recieved, const int cost) {
  std::string s_id = buildingString + std::to_string(id);
  DataStore::addValue(s_id + "_recieved", recieved);
  DataStore::addValue(s_id + "_cost", cost);
}

void Building_Appliances::addAppToDataStrore(const int id) {
  std::string s_id = buildingString + std::to_string(id);
  DataStore::addVariable(s_id + "_supplied");
  DataStore::addVariable(s_id + "_recieved");
  DataStore::addVariable(s_id + "_requested");
  DataStore::addVariable(s_id + "_cost");
}
