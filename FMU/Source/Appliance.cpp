// Copyright 2016 Jacob Chapman

#include <string>
#include <vector>
#include "Utility.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Appliance.h"

Appliance::Appliance() {}

double Appliance::getSupply() const {
  return parameters.supply;
}

double Appliance::getSupplyCost() const {
  return parameters.supplyCost;
}

double Appliance::getPower() const {
  return parameters.power;
}

double Appliance::getReceived() const {
  return parameters.received;
}

double Appliance::getReceivedCost() const {
  return parameters.receivedCost;
}

double Appliance::getPriority() const {
  int timeStep = SimulationConfig::getStepCount();
  int leninsec = SimulationConfig::lengthOfTimestep();
  int numberOfSeconds = timeStep * leninsec;
  int hour = numberOfSeconds / 3600;
  int hourOfDay = hour % 24;
  return hourlyPriority[hourOfDay];
}

void Appliance::setReceived(const double r) {
  parameters.received = r;
}

void Appliance::setReceivedCost(const double c) {
  parameters.receivedCost = c;
}

void Appliance::setupSave() {
  datastoreIDSupplied = DataStore::addVariable(idString + "_supplied");
  datastoreIDSuppliedCost = DataStore::addVariable(idString + "_suppliedCost");
  datastoreIDReceived = DataStore::addVariable(idString + "_received");
  datastoreIDRequested = DataStore::addVariable(idString + "_requested");
  datastoreIDCost = DataStore::addVariable(idString + "_cost");

  datastoreLocalIDSupplied = DataStore::addVariable(idString + "_supplied_local");
  datastoreLocalIDReceived = DataStore::addVariable(idString + "_received_local");
  datastoreLocalIDRequested = DataStore::addVariable(idString + "_requested_local");
  datastoreLocalIDCost = DataStore::addVariable(idString + "_cost_local");

  datastoreNeighbourhoodIDSupplied = DataStore::addVariable(idString + "_supplied_neighbourhood");
  datastoreNeighbourhoodIDReceived = DataStore::addVariable(idString + "_received_neighbourhood");
  datastoreNeighbourhoodIDRequested = DataStore::addVariable(idString + "_requested_neighbourhood");
  datastoreNeighbourhoodIDCost = DataStore::addVariable(idString + "_cost_neighbourhood");

  datastoreGridIDSupplied = DataStore::addVariable(idString + "_supplied_grid");
  datastoreGridIDReceived = DataStore::addVariable(idString + "_received_grid");
  datastoreGridIDRequested = DataStore::addVariable(idString + "_requested_grid");
  datastoreGridIDCost = DataStore::addVariable(idString + "_cost_grid");
}

void Appliance::save() {
  DataStore::addValue(datastoreIDSupplied, parameters.supply);
  DataStore::addValue(datastoreIDReceived, parameters.received);
  DataStore::addValue(datastoreIDRequested, parameters.power);
  DataStore::addValue(datastoreIDCost, parameters.receivedCost);
}

void Appliance::saveLocal() {
  parametersLocal.supply = parameters.supply - parameters.suppliedLeft;
  parametersLocal.suppliedLeft = parameters.suppliedLeft;
  parametersLocal.received = parameters.received;
  parametersLocal.power = parameters.power;
  parametersLocal.receivedCost = parameters.receivedCost;

  DataStore::addValue(datastoreLocalIDSupplied, parametersLocal.supply);
  DataStore::addValue(datastoreLocalIDReceived, parametersLocal.received);
  DataStore::addValue(datastoreLocalIDRequested, parametersLocal.power);
  DataStore::addValue(datastoreLocalIDCost, parametersLocal.receivedCost);
}

void Appliance::saveNeighbourhood() {
  parametersNeighbourhood.supply = parametersLocal.suppliedLeft - parameters.suppliedLeft;
  parametersNeighbourhood.suppliedLeft = parameters.suppliedLeft;
  parametersNeighbourhood.received = parameters.received - parametersLocal.received;
  parametersNeighbourhood.power =  parameters.power - parametersLocal.received;
  parametersNeighbourhood.receivedCost =  parameters.receivedCost - parametersLocal.receivedCost;

  DataStore::addValue(datastoreNeighbourhoodIDSupplied, parametersNeighbourhood.supply);
  DataStore::addValue(datastoreNeighbourhoodIDReceived, parametersNeighbourhood.received);
  DataStore::addValue(datastoreNeighbourhoodIDRequested, parametersNeighbourhood.power);
  DataStore::addValue(datastoreNeighbourhoodIDCost, parametersNeighbourhood.receivedCost);
}

void Appliance::saveGlobal() {
  parametersGrid.supply = parametersNeighbourhood.suppliedLeft - parameters.suppliedLeft;
  parametersGrid.suppliedLeft = parameters.suppliedLeft;
  parametersGrid.received = parameters.received - parametersNeighbourhood.received - parametersLocal.received;
  parametersGrid.power =  parameters.power - parametersNeighbourhood.received - parametersLocal.received;
  parametersGrid.receivedCost =  parameters.receivedCost - parametersNeighbourhood.receivedCost  - parametersLocal.receivedCost;

  DataStore::addValue(datastoreGridIDSupplied, parametersGrid.supply);
  DataStore::addValue(datastoreGridIDReceived, parametersGrid.received);
  DataStore::addValue(datastoreGridIDRequested, parametersGrid.power);
  DataStore::addValue(datastoreGridIDCost, parametersGrid.receivedCost);
}

void Appliance::clear() {
  local = false;
  global = false;
  parameters.power = 0.0;
  parameters.supply = 0.0;
  parameters.supplyCost = 0.0;
  parameters.received = 0.0;
  parameters.receivedCost = 0.0;
}

bool Appliance::isLocal() const {
  return local;
}

void Appliance::setLocal(bool local) {
  this->local = local;
}

bool Appliance::isGlobal() const {
  return global;
}

void Appliance::setGlobal(bool global) {
  this->global = global;
}
void Appliance::setPower(const double power) {
  this->parameters.power = power;
}

void Appliance::setSupply(const double supply) {
  this->parameters.supply = supply;
}

void Appliance::setSupplyCost(const double supplyCost) {
  this->parameters.supplyCost = supplyCost;
}

void Appliance::setHourlyCost(const std::vector<double> & cost) {
  hourlyCost = cost;
}

void Appliance::setHoulyPriority(const std::vector<double> & priority) {
  this->hourlyPriority = priority;
}

void Appliance::setSupplyLeft(const double supplyLeft) {
  this->parameters.suppliedLeft = supplyLeft;
}

int Appliance::calculateHourOfDay() const {
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  return hour % 24;
}
