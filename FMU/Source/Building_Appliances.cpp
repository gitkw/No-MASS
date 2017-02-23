// Copyright 2016 Jacob Chapman

#include <string>
#include <list>
#include <vector>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building_Appliances.h"

Building_Appliances::Building_Appliances() {}

void Building_Appliances::setup(const buildingStruct & b) {
  buildingID = b.id;
  buildingString = "Building" + std::to_string(buildingID) + "_Appliance";
  batteriesGrid.setup(b.AppliancesBatteryGrid, buildingID, buildingString);
  batteries.setup(b.AppliancesBattery, buildingID, buildingString);
  csv.setup(b.AppliancesCSV, buildingID, buildingString);
  large.setup(b.AppliancesLarge, buildingID, buildingString);
  largeLearning.setup(b.AppliancesLargeLearning, buildingID, buildingString);
  small.setup(b.AppliancesSmall, buildingID, buildingString);
  fmi.setup(b.AppliancesFMI, buildingID, buildingString);

  datastoreIDNonShiftSupplied = DataStore::addVariable(buildingString + "_NonShift_supplied");
  datastoreIDNonShiftSuppliedCost = DataStore::addVariable(buildingString + "_NonShift_suppliedCost");
  datastoreIDNonShiftReceived = DataStore::addVariable(buildingString + "_NonShift_received");
  datastoreIDNonShiftRequested = DataStore::addVariable(buildingString + "_NonShift_requested");
  datastoreIDNonShiftCost = DataStore::addVariable(buildingString + "_NonShift_cost");

}

void Building_Appliances::preprocess() {}

void Building_Appliances::stepLocal() {
  large.step(&app_negotiation);
  largeLearning.step(&app_negotiation);
  small.step(&app_negotiation);
  csv.step(&app_negotiation);
  fmi.step(&app_negotiation);

  double powerShortage = large.getPower()
                  + largeLearning.getPower()
                  + small.getPower()
                  + csv.getPower()
                  + fmi.getPower()
                  - large.getSupply()
                  - largeLearning.getSupply()
                  - small.getSupply()
                  - csv.getSupply()
                  - fmi.getSupply();

    batteries.setPowerShortage(powerShortage);
    batteries.step(&app_negotiation);
    powerShortage = batteries.getPowerShortage();
    batteriesGrid.setPowerShortage(powerShortage);
    batteriesGrid.step(&app_negotiation);
}

void Building_Appliances::stepLocalNegotiation() {
  app_negotiation.process();
  small.localNegotiation(app_negotiation);
  large.localNegotiation(app_negotiation);
  largeLearning.localNegotiation(app_negotiation);
  fmi.localNegotiation(app_negotiation);
  csv.localNegotiation(app_negotiation);
  batteries.localNegotiation(app_negotiation);
  batteriesGrid.localNegotiation(app_negotiation);
  app_negotiation.clear();
}

void Building_Appliances::stepNeighbourhoodNegotiation(
                          const Contract_Negotiation & building_negotiation) {

    large.neighbourhoodNegotiation(building_negotiation);
    largeLearning.neighbourhoodNegotiation(building_negotiation);
    small.neighbourhoodNegotiation(building_negotiation);
    fmi.neighbourhoodNegotiation(building_negotiation);
    csv.neighbourhoodNegotiation(building_negotiation);

    batteries.neighbourhoodNegotiation(building_negotiation);
    batteriesGrid.neighbourhoodNegotiation(building_negotiation);

}

void Building_Appliances::stepGlobalNegotiation(
                          const Contract_Negotiation & building_negotiation) {

  large.globalNegotiation(building_negotiation);
  largeLearning.globalNegotiation(building_negotiation);
  small.globalNegotiation(building_negotiation);
  fmi.globalNegotiation(building_negotiation);
  csv.globalNegotiation(building_negotiation);

  double cost = large.getReceivedCost() +
                largeLearning.getReceivedCost() +
                small.getReceivedCost() +
                fmi.getReceivedCost() +
                csv.getReceivedCost();

  batteries.globalCost(cost);
  batteriesGrid.globalCost(cost);
  batteries.globalNegotiation(building_negotiation);
  batteriesGrid.globalNegotiation(building_negotiation);

  double supply = large.getSupply() +
          small.getSupply() +
          fmi.getSupply() +
          csv.getSupply();
  double supplyCost = large.getSupplyCost() +
          small.getSupplyCost() +
          fmi.getSupplyCost() +
          csv.getSupplyCost();
  double received = large.getReceived() +
          small.getReceived() +
          fmi.getReceived() +
          csv.getReceived();
  double power = large.getPower() +
          small.getPower() +
          fmi.getPower() +
          csv.getPower();
  double receivedCost = large.getReceivedCost() +
          small.getReceivedCost() +
          fmi.getReceivedCost() +
          csv.getReceivedCost();


  DataStore::addValue(datastoreIDNonShiftSupplied, supply);
  DataStore::addValue(datastoreIDNonShiftSuppliedCost, supplyCost);
  DataStore::addValue(datastoreIDNonShiftReceived, received);
  DataStore::addValue(datastoreIDNonShiftRequested, power);
  DataStore::addValue(datastoreIDNonShiftCost, receivedCost);

  large.reset();
  largeLearning.reset();
  small.reset();
  csv.reset();
  fmi.reset();
  batteries.reset();
  batteriesGrid.reset();

  currentStates.clear();
  app_negotiation.clear();
}

void Building_Appliances::postprocess() {
  largeLearning.postprocess();
  batteries.postprocess();
  batteriesGrid.postprocess();
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
                                Contract_Negotiation * building_negotiation,
                              const bool battery) const {
  large.addGlobalContactsTo(building_negotiation);
  largeLearning.addGlobalContactsTo(building_negotiation);
  small.addGlobalContactsTo(building_negotiation);
  csv.addGlobalContactsTo(building_negotiation);
  fmi.addGlobalContactsTo(building_negotiation);
  if (battery) {
    batteries.addGlobalContactsTo(building_negotiation);
    batteriesGrid.addGlobalContactsTo(building_negotiation);
  }
}
