// Copyright 2016 Jacob Chapman

#include <string>
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_PV.h"

Appliance_PV::Appliance_PV() {}

/**
 * @brief read in the csv file containing the power values
 */
void Appliance_PV::setup() {
  model.parseConfiguration(SimulationConfig::RunLocation + filename);
}

/**
 * @brief retrieves power values for each timestep
 */
void Appliance_PV::preprocess() {
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
  int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;
  int leninsec = SimulationConfig::lengthOfTimestep();
  for (int timeStep = 0; timeStep < totoaltimesteps; timeStep++) {
    int numberOfSeconds = timeStep * leninsec;
    int minute = numberOfSeconds / 60;
    int minuteOfDay = minute % 1440;
    int hour = numberOfSeconds / 3600;
    int day = hour / 24;
    int now = numberOfSeconds;
    int end = numberOfSeconds + leninsec;
    double p = 0;
    while (now < end) {
      p += model.supply(day, minuteOfDay);
      minuteOfDay += 1;
      if (minuteOfDay >= 1440) {
        minuteOfDay = 0;
        day +=1;
      }
      now += 60;
    }
    // watt hour per timeperhour
    power.push_back(0);
    supply.push_back(p);
  }
}

/**
 * @brief Sets the csv file to be read in.
 * @param filename: the file name
 */
void Appliance_PV::setFileName(std::string filename) {
  this->filename = filename;
}
