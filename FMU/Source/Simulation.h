// Copyright 2016 Jacob Chapman

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <string>
#include <vector>
#include "LVN.h"
#include "Building.h"
#include "Log.h"

/**
 * @brief Main NoMASS simulation manager
 * @details Called through the FMU interface, manages the simulation of the NoMass platform
 */
class Simulation {
 public:
    Simulation();

    void preprocess();
    void parseConfiguration(const std::string & file);
    void setupSimulationModel();
    void postprocess();
    void preTimeStep();
    void timeStep();
    void postTimeStep();

    void setSimulationConfigurationFile(const std::string & filename);
    static double getGridCost();

 private:
    std::vector<int> monthCount;
    std::string simulationConfigurationFile;
    static double gridCost;
    /**
    * The Buildings
    */
    std::vector<Building> buildings;
    /**
    * Manages the low voltage network
    */
    LVN lvn;
    /**
    * Manages the building power negotiation
    */
    Contract_Negotiation building_negotiation;

    int GridPowerDataId;
    int GridCostDataId;
};

#endif  // SIMULATION_H_
