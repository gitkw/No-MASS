// Copyright 2016 Jacob Chapman

#include <string>
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

double Appliance::powerBack() const {
  return power.back();
}

double Appliance::supplyBack() const {
  return supply.back();
}

double Appliance::getPriority() const {
  return priority;
}

void Appliance::setPriority(double priority) {
  this->priority = priority;
}

double Appliance::getCost() const {
  return cost;
}

void Appliance::setCost(double cost) {
  this->cost = cost;
}

void Appliance::setRecieved(const double r) {
  recieved.push_back(r);
}

void Appliance::setRecievedCost(const double c) {
  recievedCost.push_back(c);
}

void Appliance::saveSetup() {
  DataStore::addVariable(idString + "_supplied");
  DataStore::addVariable(idString + "_recieved");
  DataStore::addVariable(idString + "_requested");
  DataStore::addVariable(idString + "_cost");
}

void Appliance::save() {
  int timestep = SimulationConfig::getStepCount();
  DataStore::addValue(idString + "_recieved", recieved[timestep]);
  DataStore::addValue(idString + "_cost", recievedCost[timestep]);
  DataStore::addValue(idString + "_supplied", supply[timestep]);
  DataStore::addValue(idString + "_requested", power[timestep]);
}


bool Appliance::isGlobal() const {
  return global;
}

void Appliance::setGlobal(bool global) {
  this->global = global;
}
