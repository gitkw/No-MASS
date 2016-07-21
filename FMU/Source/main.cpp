// Copyright 2016 Jacob Chapman

#include "Utility.h"
#include "Simulation.h"

Simulation sim;

int main(int argc,char *argv[]){
  if (argc > 1){
    sim.setSimulationConfigurationFile(argv[1]);
  }

    sim.preprocess();

    int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                              SimulationConfig::info.startMonth,
                                              SimulationConfig::info.endDay,
                                              SimulationConfig::info.endMonth);
    int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;
    for(int i = 0; i < totoaltimesteps; i++){
        sim.preTimeStep();
        sim.timeStep();
        sim.postTimeStep();
    }
    sim.postprocess();
}
