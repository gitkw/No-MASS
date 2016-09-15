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

void Building_Appliances::setup(const buildingStruct & b) {
  buildingID = b.id;
  buildingString = "Building" + std::to_string(buildingID) + "_Appliance";
  DataStore::addVariable(buildingString + "_Sum_Recieved");
  DataStore::addVariable(buildingString + "_Sum_Small");
  DataStore::addVariable(buildingString + "_Sum_Large");
  DataStore::addVariable(buildingString + "_Sum_Cost");

  std::vector<appCSVStruct> appCSV =
                  b.AppliancesCSV;
  for (const appCSVStruct s : appCSV) {
    csv.push_back(Appliance_Generic_CSV());
    csv.back().setID(s.id);
    csv.back().setHoulyPriority(s.priority);
    csv.back().setHourlyCost(s.cost);
    csv.back().setBuildingID(buildingID);
    csv.back().setFileDemand(s.fileDemand);
    csv.back().setFileSupply(s.fileSupply);
    csv.back().setup();
    csv.back().setIDString(buildingString + std::to_string(s.id));
    csv.back().setupSave();
    csv.back().setupPriority();
  }

  std::vector<appLargeStruct> app =
                  b.AppliancesLarge;
  for (const appLargeStruct &s : app) {
    large.push_back(Appliance_Large());
    large.back().setID(s.id);
    large.back().setHoulyPriority(s.priority);
    large.back().setBuildingID(buildingID);
    large.back().setActivities(s.activities);
    large.back().setup();
    large.back().setIDString(buildingString + std::to_string(s.id));
    large.back().setupSave();
    large.back().setupPriority();
  }

  app = b.AppliancesLargeLearning;
  for (const appLargeStruct &s : app) {
    largeLearning.push_back(Appliance_Large_Learning());
    largeLearning.back().setEpsilon(s.epsilon);
    largeLearning.back().setAlpha(s.alpha);
    largeLearning.back().setGamma(s.gamma);
    largeLearning.back().setUpdate(s.update);
    largeLearning.back().setID(s.id);
    largeLearning.back().setHoulyPriority(s.priority);
    largeLearning.back().setBuildingID(buildingID);
    largeLearning.back().setActivities(s.activities);
    largeLearning.back().setup();
    largeLearning.back().setIDString(buildingString + std::to_string(s.id));
    largeLearning.back().setupSave();
    largeLearning.back().setupPriority();
  }

  std::vector<appSmallStruct> appSmall =
                  b.AppliancesSmall;
  for (const appSmallStruct s : appSmall) {
    small.push_back(Appliance_Small());
    small.back().setID(s.id);
    small.back().setHoulyPriority(s.priority);
    small.back().setWeibullParameters(s.WeibullParameters);
    small.back().setStateProbabilities(s.StateProbabilities);
    small.back().setFractions(s.Fractions);
    small.back().setSumRatedPowers(s.SumRatedPowers);
    small.back().setup();
    small.back().setIDString(buildingString + std::to_string(s.id));
    small.back().setupSave();
    small.back().setupPriority();
  }

  std::vector<appFMIStruct> appFMI =
                  b.AppliancesFMI;
  for (const appFMIStruct s : appFMI) {
    fmi.push_back(Appliance_FMI());
    fmi.back().setID(s.id);
    fmi.back().setHoulyPriority(s.priority);
    fmi.back().setFMIVariableName(s.variableName);
    fmi.back().setup();
    fmi.back().setIDString(buildingString + std::to_string(s.id));
    fmi.back().setupSave();
    fmi.back().setupPriority();
  }
}

void Building_Appliances::preprocess() {
  std::vector<int> pop = Utility::randomIntVect(small.size());
  for (int a : pop) {
      small[a].preprocess();
  }
  pop = Utility::randomIntVect(csv.size());
  for (int a : pop) {
      csv[a].preprocess();
  }
}

bool Building_Appliances::sendContractGlobal(const contract & c) {
  bool send = (c.requested > c.recieved || c.suppliedLeft > 0);
  if (send) {
    contract x;
    x.id = c.id;
    x.buildingID = buildingID;
    x.recievedCost = c.recievedCost;
    x.requested = c.requested - c.recieved;
    x.recieved = c.recieved;
    x.supplied = c.suppliedLeft;
    x.suppliedCost = c.suppliedCost;
    globalContracts.push_back(x);
  }
  return send;
}

void Building_Appliances::sendContractLocal(const Appliance & a) {
  int stepcount = SimulationConfig::getStepCount();
  contract c;
  c.id = a.getID();
  c.buildingID = buildingID;
  c.requested = a.powerAt(stepcount);
  c.priority = a.getPriorityAt(stepcount);
  c.supplied = a.supplyAt(stepcount);
  c.suppliedCost = a.supplyCostAt(stepcount);
  c.recievedCost = 0;
  c.recieved = 0;
  app_negotiation.submit(c);
  PowerRequested += c.requested;
  PowerGenerated += c.supplied;
}

void Building_Appliances::stepLocalLarge() {
  std::vector<int> pop = Utility::randomIntVect(large.size());
  for (int a : pop) {
    large[a].hasActivities(currentStates);
    large[a].step();
    sendContractLocal(large[a]);
  }
}

void Building_Appliances::stepLocalLargeLearning() {
  std::vector<int> pop = Utility::randomIntVect(largeLearning.size());
  for (int a : pop) {
    largeLearning[a].hasActivities(currentStates);
    largeLearning[a].step();
    sendContractLocal(largeLearning[a]);
  }
}

void Building_Appliances::stepLocalSmall() {
  std::vector<int> pop = Utility::randomIntVect(small.size());
  for (int a : pop) {
    sendContractLocal(small[a]);
  }
}

void Building_Appliances::stepLocalCSV() {
  std::vector<int> pop = Utility::randomIntVect(csv.size());
  for (int a : pop) {
    sendContractLocal(csv[a]);
  }
}

void Building_Appliances::stepLocalFMI() {
  std::vector<int> pop = Utility::randomIntVect(fmi.size());
  for (int a : pop) {
    fmi[a].step();
    sendContractLocal(fmi[a]);
  }
}

void Building_Appliances::stepLocal() {
  PowerRequested = 0;
  PowerGenerated = 0;
  stepLocalCSV();
  stepLocalLarge();
  stepLocalSmall();
  stepLocalFMI();
  stepLocalLargeLearning();
}


void Building_Appliances::stepLocalNegotiation() {
  sum_large = 0;
  sum_cost = 0;
  sum_fmi = 0;
  sum_small = 0;
  app_negotiation.process();
  localNegotiationSmall();
  localNegotiationLarge();
  localNegotiationLargeLearning();
  localNegotiationFMI();
  localNegotiationCSV();
  app_negotiation.clear();
}

void Building_Appliances::globalNegotiationSmall(
                                const LVN_Negotiation & building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(small.size());
  for (int a : pop) {
    if (small[a].isGlobal()) {
      int appid = small[a].getID();
      contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.recieved;
      double cost = c.recievedCost;
      small[a].setRecieved(power);
      small[a].setRecievedCost(cost);
      small[a].save();
      sum_small += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::localNegotiationSmall() {
  std::vector<int> pop = Utility::randomIntVect(small.size());
  for (int a : pop) {
    int appid = small[a].getID();
    contract c = app_negotiation.getContract(buildingID, appid);
    small[a].setGlobal(sendContractGlobal(c));
    if (!small[a].isGlobal()) {
      double power = c.recieved;
      double cost = c.recievedCost;
      small[a].setRecieved(power);
      small[a].setRecievedCost(cost);
      small[a].save();
      sum_small += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::globalNegotiationLarge(const LVN_Negotiation & building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(large.size());
  for (int a : pop) {
    if (large[a].isGlobal()) {
      int appid = large[a].getID();
      contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.recieved;
      double cost = c.recievedCost;
      large[a].setRecieved(power);
      large[a].setRecievedCost(cost);
      large[a].save();
      sum_large += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::localNegotiationLarge() {
  std::vector<int> pop = Utility::randomIntVect(large.size());
  for (int a : pop) {
    int appid = large[a].getID();
    contract c = app_negotiation.getContract(buildingID, appid);
    large[a].setGlobal(sendContractGlobal(c));
    if (!large[a].isGlobal()) {
      double power = c.recieved;
      double cost = c.recievedCost;
      large[a].setRecieved(power);
      large[a].setRecievedCost(cost);
      large[a].save();
      sum_large += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::globalNegotiationLargeLearning(
                                const LVN_Negotiation & building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(largeLearning.size());
  for (int a : pop) {
    if (largeLearning[a].isGlobal()) {
      int appid = largeLearning[a].getID();
      contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.recieved;
      double cost = c.recievedCost;
      largeLearning[a].addToCost(cost);
      largeLearning[a].setRecieved(power);
      largeLearning[a].setRecievedCost(cost);
      largeLearning[a].save();
      sum_large += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::localNegotiationLargeLearning() {
  std::vector<int> pop = Utility::randomIntVect(largeLearning.size());
  for (int a : pop) {
    int appid = largeLearning[a].getID();
    contract c = app_negotiation.getContract(buildingID, appid);
    largeLearning[a].setGlobal(sendContractGlobal(c));
    if (!largeLearning[a].isGlobal()) {
      double power = c.recieved;
      double cost = c.recievedCost;
      largeLearning[a].addToCost(cost);
      largeLearning[a].setRecieved(power);
      largeLearning[a].setRecievedCost(cost);
      largeLearning[a].save();
      sum_large += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::globalNegotiationCSV(
                                const LVN_Negotiation & building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(csv.size());
  for (int a : pop) {
    if (csv[a].isGlobal()) {
    int appid = csv[a].getID();
    contract c = building_negotiation.getContract(buildingID, appid);
    double power = c.recieved;
    double cost = c.recievedCost;
    csv[a].setRecieved(power);
    csv[a].setRecievedCost(cost);
    csv[a].save();
    }
  }
}

void Building_Appliances::localNegotiationCSV() {
  std::vector<int> pop = Utility::randomIntVect(csv.size());
  for (int a : pop) {
    int appid = csv[a].getID();
    contract c = app_negotiation.getContract(buildingID, appid);
    csv[a].setGlobal(sendContractGlobal(c));
    if (!csv[a].isGlobal()) {
      double power = c.recieved;
      double cost = c.recievedCost;
      csv[a].setRecieved(power);
      csv[a].setRecievedCost(cost);
      csv[a].save();
    }
  }
}

void Building_Appliances::globalNegotiationFMI(
                                const LVN_Negotiation & building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(fmi.size());
  for (int a : pop) {
    if (fmi[a].isGlobal()) {
    int appid = fmi[a].getID();
    contract c = building_negotiation.getContract(buildingID, appid);
    double power = c.recieved;
    double cost = c.recievedCost;
    fmi[a].setRecieved(power);
    fmi[a].setRecievedCost(cost);
    fmi[a].save();
    sum_fmi += power;
    sum_cost += cost;
    }
  }
}

void Building_Appliances::localNegotiationFMI() {
  std::vector<int> pop = Utility::randomIntVect(fmi.size());
  for (int a : pop) {
    int appid = fmi[a].getID();
    contract c = app_negotiation.getContract(buildingID, appid);
    fmi[a].setGlobal(sendContractGlobal(c));
    if (!fmi[a].isGlobal()) {
      double power = c.recieved;
      double cost = c.recievedCost;
      fmi[a].setRecieved(power);
      fmi[a].setRecievedCost(cost);
      fmi[a].save();
      sum_fmi += power;
      sum_cost += cost;
    }
  }
}

void Building_Appliances::stepGlobalNegotiation(
                                const LVN_Negotiation & building_negotiation) {
  globalNegotiationLarge(building_negotiation);
  globalNegotiationLargeLearning(building_negotiation);
  globalNegotiationSmall(building_negotiation);
  globalNegotiationFMI(building_negotiation);
  globalNegotiationCSV(building_negotiation);
  globalContracts.clear();
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
  std::vector<int> pop = Utility::randomIntVect(largeLearning.size());
  for (int a : pop) {
    largeLearning[a].postprocess();
  }
}

void Building_Appliances::postTimeStep() {
}

double Building_Appliances::getTotalPower() const {
  return totalPower;
}

void Building_Appliances::addCurrentStates(const int stateid) {
    currentStates.push_back(stateid);
}

void Building_Appliances::addContactsTo(
                                      LVN_Negotiation * building_negotiation) {
  for (const contract & c : globalContracts) {
    building_negotiation->submit(c);
  }
}
