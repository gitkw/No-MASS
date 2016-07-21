// Copyright 2015 Jacob Chapman


#ifndef SIMULATION_H
#define	SIMULATION_H

#include <string>
#include <vector>
#include "Building.h"
#include "Log.h"

/**
 * @brief Main NoMASS simulation manager
 * @details Called through the FMU interface, manages the simulation of the NoMass platform
 */
class Simulation {

public:
    Simulation();
    ~Simulation(){}

    void preprocess();
    void parseConfiguration(const std::string file);
    void setupSimulationModel();
    void postprocess();
    void preTimeStep();
    void timeStep();
    void postTimeStep();

    void setSimulationConfigurationFile(const std::string & filename);

private:
    std::vector<int> monthCount;
    std::string simulationConfigurationFile;

    /**
     * Manages the Buildings
     */
    std::vector<Building> buildings;

};

#endif	/* SIMULATION_H */
