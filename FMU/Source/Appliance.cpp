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
}

void Appliance::save() {
  DataStore::addValue(datastoreIDSupplied, parameters.supply);
  DataStore::addValue(datastoreIDSuppliedCost, parameters.supplyCost);
  DataStore::addValue(datastoreIDReceived, parameters.received);
  DataStore::addValue(datastoreIDRequested, parameters.power);
  DataStore::addValue(datastoreIDCost, parameters.receivedCost);
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

int Appliance::calculateHourOfDay() const {
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  return hour % 24;
}
