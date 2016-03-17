// Copyright 2015 Jacob Chapman

#include <iostream>
#include <string>

#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building.h"
#include "Simulation.h"

Simulation::Simulation() {
        monthCount.push_back(31);
        monthCount.push_back(59);
        monthCount.push_back(90);
        monthCount.push_back(120);
        monthCount.push_back(151);
        monthCount.push_back(181);
        monthCount.push_back(212);
        monthCount.push_back(243);
        monthCount.push_back(273);
        monthCount.push_back(304);
        monthCount.push_back(334);
        monthCount.push_back(365);
}


/**
 * @brief Calls the simulation preprocess
 * @details Reads in the configuration file and sends to parser.
 * Sets up the EnergyPlus processor, the AgentModel and the ZoneManager.
 */
void Simulation::preprocess() {
  parseConfiguration(SimulationConfig::FmuLocation
    + "/SimulationConfig.xml");
  if (!LOG.getError()) {
    setupSimulationModel();
  }
}

void Simulation::parseConfiguration(const std::string file) {
    SimulationConfig::parseConfiguration(file);
}

void Simulation::setupSimulationModel() {
    SimulationConfig::stepCount = -1;
    DataStore::addVariable("day");
    DataStore::addVariable("month");
    DataStore::addVariable("hour");
    DataStore::addVariable("hourOfDay");
    DataStore::addVariable("TimeStep");
    for (buildingStruct b : SimulationConfig::buildings) {
      buildings.push_back(Building());
      buildings.back().setup(b);
    }
}

/**
 * @brief Calls the simulation prostprocess
 *
 */
void Simulation::postprocess() {
  for (Building &b : buildings) {
    b.postprocess();
  }
  DataStore::print();
  DataStore::clear();
}

/**
 * @brief processes before timestep
 */
void Simulation::preTimeStep() {
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  int day = hour / 24;

  int month = 1;
  for (int mc : monthCount) {
    if (mc > day || month + 1 > 12) {
      break;
    }
    month = month + 1;
  }
  /*int hourOfDay = 0;
  for (int i = 0; i <= hour; i++) {
    hourOfDay += 1;
    if (hourOfDay > 23) {
      hourOfDay = 0;
    }
  }*/
  int hourOfDay = hour % 24;
  DataStore::addValue("TimeStep", stepCount);
  DataStore::addValue("day", day);
  DataStore::addValue("hour", hour);
  DataStore::addValue("hourOfDay", hourOfDay);
  DataStore::addValue("month", month);
}

/**
 * @brief Increments the timestep for the simulation
 * @details Increments the timestep for the EnergyPlus processor, the AgentModel and the ZoneManager.
 * Also we send any effects the agent have to the zones they are located in.
 */
void Simulation::timeStep() {
        SimulationConfig::step();

        for (Building &b : buildings) {
            b.step();
        }
}

/**
 * @brief processes After timestep
 */
void Simulation::postTimeStep() {
}
