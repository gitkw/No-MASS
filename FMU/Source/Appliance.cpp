// Copyright 2016 Jacob Chapman

#include <string>
#include <vector>
#include "Utility.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Appliance.h"

Appliance::Appliance() {}

double Appliance::powerAt(const int timestep) const {
  return power[timestep];
}

double Appliance::supplyAt(const int timestep) const {
  return supply[timestep];
}

double Appliance::supplyCostAt(const int timestep) const {
  return supplyCost[timestep];
}

double Appliance::powerBack() const {
  return power.back();
}

double Appliance::supplyBack() const {
  return supply.back();
}

double Appliance::getPriorityAt(const int timestep) const {
  return priority[timestep];
}

void Appliance::setRecieved(const double r) {
  recieved.push_back(r);
}

void Appliance::setRecievedCost(const double c) {
  recievedCost.push_back(c);
}

void Appliance::setupSave() {
  DataStore::addVariable(idString + "_supplied");
  DataStore::addVariable(idString + "_suppliedCost");
  DataStore::addVariable(idString + "_recieved");
  DataStore::addVariable(idString + "_requested");
  DataStore::addVariable(idString + "_cost");
}

void Appliance::setupPriority() {
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
  int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;
  int leninsec = SimulationConfig::lengthOfTimestep();
  for (int timeStep = 0; timeStep < totoaltimesteps; timeStep++) {
    int numberOfSeconds = timeStep * leninsec;
    int hour = numberOfSeconds / 3600;
    int hourOfDay = hour % 24;
    priority.push_back(hourlyPriority[hourOfDay]);
  }
}

void Appliance::save() {
  int timestep = SimulationConfig::getStepCount();
  DataStore::addValue(idString + "_recieved", recieved[timestep]);
  DataStore::addValue(idString + "_cost", recievedCost[timestep]);
  DataStore::addValue(idString + "_supplied", supply[timestep]);
  DataStore::addValue(idString + "_suppliedCost", supplyCost[timestep]);
  DataStore::addValue(idString + "_requested", power[timestep]);
}


bool Appliance::isGlobal() const {
  return global;
}

void Appliance::setGlobal(bool global) {
  this->global = global;
}

void Appliance::setHourlyCost(const std::vector<double> & cost) {
  hourlyCost = cost;
}

void Appliance::setHoulyPriority(const std::vector<double> & priority) {
  this->hourlyPriority = priority;
}
