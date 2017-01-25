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

    std::vector<appBatteryStruct> appBattery =
                    b.AppliancesBattery;
    for (const appBatteryStruct s : appBattery) {
      batteries.push_back(Appliance_Battery());
      batteries.back().setID(s.id);
      batteries.back().setEpsilon(s.epsilon);
      batteries.back().setAlpha(s.alpha);
      batteries.back().setGamma(s.gamma);
      batteries.back().setUpdate(s.update);
      batteries.back().setHoulyPriority(s.priority);
      batteries.back().setBuildingID(buildingID);
      batteries.back().setup();
      batteries.back().setIDString(buildingString + std::to_string(s.id));
      batteries.back().setupSave();
    }

    std::vector<appBatteryStruct> appBatteryGrid =
                    b.AppliancesBatteryGrid;
    for (const appBatteryStruct s : appBatteryGrid) {
      batteriesGrid.push_back(Appliance_Battery_GridCost_Reward());
      batteriesGrid.back().setID(s.id);
      batteriesGrid.back().setEpsilon(s.epsilon);
      batteriesGrid.back().setAlpha(s.alpha);
      batteriesGrid.back().setGamma(s.gamma);
      batteriesGrid.back().setUpdate(s.update);
      batteriesGrid.back().setHoulyPriority(s.priority);
      batteriesGrid.back().setBuildingID(buildingID);
      batteriesGrid.back().setup();
      batteriesGrid.back().setIDString(buildingString + std::to_string(s.id));
      batteriesGrid.back().setupSave();
    }

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
    largeLearning.back().setHoulyTimeRequired(s.timeRequired);
    largeLearning.back().setBuildingID(buildingID);
    largeLearning.back().setActivities(s.activities);
    largeLearning.back().setup();
    largeLearning.back().setIDString(buildingString + std::to_string(s.id));
    largeLearning.back().setupSave();
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
  }
}

void Building_Appliances::preprocess() {}

bool Building_Appliances::sendContractGlobal(const Contract & c) {
  bool send = (c.requested > c.received || c.suppliedLeft > 0);
  if (send) {
    Contract x;
    x.id = c.id;
    x.buildingID = buildingID;
    x.receivedCost = c.receivedCost;
    x.requested = c.requested;
    x.received = c.received;
    x.supplied = c.suppliedLeft;
    x.priority = c.priority;
    x.suppliedCost = c.suppliedCost;
    globalContracts.push_back(x);
  }
  return send;
}

bool Building_Appliances::sendContractLocal(const Appliance & a) {
  bool ret = (a.getPower() > 0 || a.getSupply() > 0);
  if (ret) {
    Contract c;
    c.id = a.getID();
    c.buildingID = buildingID;
    c.requested = a.getPower();
    c.priority = a.getPriority();
    c.supplied = a.getSupply();
    c.suppliedCost = a.getSupplyCost();
    c.receivedCost = 0;
    c.received = 0;
    app_negotiation.submit(c);
    PowerRequested += c.requested;
    PowerGenerated += c.supplied;
  }
  return ret;
}

void Building_Appliances::stepLocalLarge() {
  std::vector<int> pop = Utility::randomIntVect(large.size());
  for (int a : pop) {
    large[a].hasActivities(currentStates);
    large[a].step();
    bool local = sendContractLocal(large[a]);
    large[a].setLocal(local);
    large[a].setGlobal(false);
  }
}

void Building_Appliances::stepLocalLargeLearning() {
  std::vector<int> pop = Utility::randomIntVect(largeLearning.size());
  for (int a : pop) {
    largeLearning[a].hasActivities(currentStates);
    largeLearning[a].step();
    bool local = sendContractLocal(largeLearning[a]);
    largeLearning[a].setLocal(local);
    largeLearning[a].setGlobal(false);
  }
}

void Building_Appliances::stepLocalSmall() {
  std::vector<int> pop = Utility::randomIntVect(small.size());
    for (int a : pop) {
        small[a].step();
    bool local = sendContractLocal(small[a]);
    small[a].setLocal(local);
    small[a].setGlobal(false);
  }
}

void Building_Appliances::stepLocalCSV() {
  std::vector<int> pop = Utility::randomIntVect(csv.size());
    for (int a : pop) {
        csv[a].step();
    bool local = sendContractLocal(csv[a]);
    csv[a].setLocal(local);
    csv[a].setGlobal(false);
  }
}

void Building_Appliances::stepLocalFMI() {
  std::vector<int> pop = Utility::randomIntVect(fmi.size());
  for (int a : pop) {
    fmi[a].step();
    bool local = sendContractLocal(fmi[a]);
    fmi[a].setLocal(local);
    fmi[a].setGlobal(false);
  }
}

void Building_Appliances::stepLocalBatteries() {
  double power = PowerRequested - PowerGenerated;
  std::vector<int> pop = Utility::randomIntVect(batteries.size());
  for (int a : pop) {
    batteries[a].setPowerShortage(power);
    batteries[a].step();
    power -= batteries[a].getSupply();
    bool local = sendContractLocal(batteries[a]);
    batteries[a].setLocal(local);
    batteries[a].setGlobal(false);
  }
  pop = Utility::randomIntVect(batteriesGrid.size());
  for (int a : pop) {
    batteriesGrid[a].setPowerShortage(power);
    batteriesGrid[a].step();
    power -= batteriesGrid[a].getSupply();
    bool local = sendContractLocal(batteriesGrid[a]);
    batteriesGrid[a].setLocal(local);
    batteriesGrid[a].setGlobal(false);
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
  stepLocalBatteries();
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
  localNegotiationBatteries();
  app_negotiation.clear();
}

void Building_Appliances::localNegotiationBatteries() {
  for (Appliance_Battery & b : batteries) {
    if (b.isLocal()) {
      int appid = b.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      b.setReceived(power);
      b.setReceivedCost(cost);
      sum_small += power;
      sum_cost += cost;
    }
    b.save();
    b.clear();
  }
  for (Appliance_Battery_GridCost_Reward & b : batteriesGrid) {
    if (b.isLocal()) {
      int appid = b.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      b.setReceived(power);
      b.setReceivedCost(cost);
      sum_small += power;
      sum_cost += cost;
    }
    b.save();
    b.clear();
  }
}

void Building_Appliances::globalNegotiationSmall(
                          const Contract_Negotiation & building_negotiation) {
  for (Appliance_Small & s : small) {
    if (s.isGlobal()) {
      int appid = s.getID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      s.setReceived(power);
      s.setReceivedCost(cost);
      sum_small += power;
      sum_cost += cost;
      sum_global_cost += cost;
    }
    s.save();
    s.clear();
  }
}

void Building_Appliances::localNegotiationSmall() {
  for (Appliance_Small & s : small) {
    if (s.isLocal()) {
      int appid = s.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      s.setGlobal(sendContractGlobal(c));
      if (!s.isGlobal()) {
        double power = c.received;
        double cost = c.receivedCost;
        s.setReceived(power);
        s.setReceivedCost(cost);
        sum_small += power;
        sum_cost += cost;
      }
    }
  }
}

void Building_Appliances::globalNegotiationLarge(
                          const Contract_Negotiation & building_negotiation) {
  for (Appliance_Large & l : large) {
    if (l.isGlobal()) {
      int appid = l.getID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      l.setReceived(power);
      l.setReceivedCost(cost);
      sum_large += power;
      sum_cost += cost;
      sum_global_cost += cost;
    }
    l.save();
    l.clear();
  }
}

void Building_Appliances::localNegotiationLarge() {
  for (Appliance_Large & l : large) {
    if (l.isLocal()) {
      int appid = l.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      l.setGlobal(sendContractGlobal(c));
      if (!l.isGlobal()) {
        double power = c.received;
        double cost = c.receivedCost;
        l.setReceived(power);
        l.setReceivedCost(cost);
        sum_large += power;
        sum_cost += cost;
      }
    }
  }
}

void Building_Appliances::globalNegotiationLargeLearning(
                          const Contract_Negotiation & building_negotiation) {
  for (Appliance_Large_Learning & l : largeLearning) {
    if (l.isGlobal()) {
      int appid = l.getID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      l.addToCost(cost);
      l.setReceived(power);
      l.setReceivedCost(cost);
      sum_large += power;
      sum_cost += cost;
      sum_global_cost += cost;
    }
    l.save();
    l.clear();
  }
}

void Building_Appliances::localNegotiationLargeLearning() {
  for (Appliance_Large_Learning & l : largeLearning) {
    if (l.isLocal()) {
      int appid = l.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      l.setGlobal(sendContractGlobal(c));
      if (!l.isGlobal()) {
        double power = c.received;
        double cost = c.receivedCost;
        l.addToCost(cost);
        l.setReceived(power);
        l.setReceivedCost(cost);
        sum_large += power;
        sum_cost += cost;
      } else {
      }
    }
  }
}

void Building_Appliances::globalNegotiationCSV(
                          const Contract_Negotiation & building_negotiation) {
  for (Appliance_Generic_CSV & g : csv) {
    if (g.isGlobal()) {
      int appid = g.getID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      g.setReceived(power);
      g.setReceivedCost(cost);
      sum_fmi += power;
      sum_cost += cost;
      sum_global_cost += cost;
    }
    g.save();
    g.clear();
  }
}

void Building_Appliances::localNegotiationCSV() {
  for (Appliance_Generic_CSV & g : csv) {
    if (g.isLocal()) {
      int appid = g.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      g.setGlobal(sendContractGlobal(c));
      if (!g.isGlobal()) {
        double power = c.received;
        double cost = c.receivedCost;
        g.setReceived(power);
        g.setReceivedCost(cost);
        sum_fmi += power;
        sum_cost += cost;
      }
    }
  }
}

void Building_Appliances::globalNegotiationFMI(
                          const Contract_Negotiation & building_negotiation) {
  for (Appliance_FMI & f : fmi) {
    if (f.isGlobal()) {
      int appid = f.getID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      double power = c.received;
      double cost = c.receivedCost;
      f.setReceived(power);
      f.setReceivedCost(cost);
      sum_fmi += power;
      sum_cost += cost;
      sum_global_cost += cost;
    }
    f.save();
    f.clear();
  }
}

void Building_Appliances::localNegotiationFMI() {
  for (Appliance_FMI & f : fmi) {
    if (f.isLocal()) {
      int appid = f.getID();
      Contract c = app_negotiation.getContract(buildingID, appid);
      f.setGlobal(sendContractGlobal(c));
      if (!f.isGlobal()) {
        double power = c.received;
        double cost = c.receivedCost;
        f.setReceived(power);
        f.setReceivedCost(cost);
        sum_fmi += power;
        sum_cost += cost;
      }
    }
  }
}

void Building_Appliances::stepGlobalNegotiation(
                          const Contract_Negotiation & building_negotiation) {
  globalNegotiationLarge(building_negotiation);
  globalNegotiationLargeLearning(building_negotiation);
  globalNegotiationSmall(building_negotiation);
  globalNegotiationFMI(building_negotiation);
  globalNegotiationCSV(building_negotiation);
  for (Appliance_Battery & b : batteries) {
    b.AddCost(sum_global_cost);
  }
  for (Appliance_Battery_GridCost_Reward & b : batteriesGrid) {
    b.AddCost(sum_global_cost);
  }
  globalContracts.clear();
  totalPower = PowerRequested - PowerGenerated;
  currentStates.clear();
  app_negotiation.clear();
  sum_global_cost = 0;
}

void Building_Appliances::postprocess() {
  for (Appliance_Large_Learning & l : largeLearning) {
    l.postprocess();
  }
  for (Appliance_Battery & b : batteries) {
    b.postprocess();
  }
  for (Appliance_Battery_GridCost_Reward & b : batteriesGrid) {
    b.postprocess();
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
                                Contract_Negotiation * building_negotiation) {
  for (const Contract & c : globalContracts) {
    building_negotiation->submit(c);
  }
}
