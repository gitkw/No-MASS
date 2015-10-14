/*
 * File:   Simulation.h
 * Author: jake
 *
 * Created on September 13, 2013, 10:12 AM
 */

#ifndef SIMULATION_H
#define	SIMULATION_H

#include <string>
#include <vector>
#include "Model_EnergyPlus.h"
#include "Model_Agents.h"
#include "Log.h"

/**
 * @brief Main NoMASS simulation manager
 * @details Called through the FMU interface, manages the simulation of the NoMass platform
 */
class Simulation {

public:
    Simulation();
    ~Simulation(){
        LOG.printLog();
    }

    void preprocess();
    void parseConfiguration(const std::string file);
    void setupSimulationModel();
    void postprocess();
    void preTimeStep();
    void timeStep();
    void postTimeStep();

private:
    /**
     * EnergyPlus model
     */
    Model_EnergyPlus energySolver;
    /**
     * Manages the Agents
     */
    Model_Agents agentModel;
    int time;
    std::vector<int> monthCount;

};

#endif	/* SIMULATION_H */
