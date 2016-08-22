// Copyright 2015 Jacob Chapman

#include <iostream>
#include <string>

#include "Environment.h"
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building.h"
#include "LVN_Node.h"
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

        simulationConfigurationFile = "SimulationConfig.xml";
}

void Simulation::setSimulationConfigurationFile(const std::string & filename) {
  this->simulationConfigurationFile = filename;
}
/**
 * @brief Calls the simulation preprocess
 * @details Reads in the configuration file and sends to parser.
 * Sets up the EnergyPlus processor, the AgentModel and the ZoneManager.
 */
void Simulation::preprocess() {
  parseConfiguration(SimulationConfig::RunLocation
    + simulationConfigurationFile);
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
      buildings.back().preprocess();
    }
    lvn.setup();
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

  int hourOfDay = hour % 24;
  DataStore::addValue("TimeStep", stepCount);
  DataStore::addValue("day", day);
  DataStore::addValue("hour", hour);
  DataStore::addValue("hourOfDay", hourOfDay);
  DataStore::addValue("month", month);
  //  std::cout << day << " " << hour << " " << hourOfDay << std::endl;

  Environment::calculateDailyMeanTemperature();
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
  postTimeStep();
}

/**
 * @brief processes After timestep
 */
void Simulation::postTimeStep() {
  for (Building &b : buildings) {
      b.postTimeStep();
      lvn.setPowerForID(b.getPower(), b.getID());
  }
  lvn.postTimeStep();
}
