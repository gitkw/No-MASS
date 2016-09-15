// Copyright 2016 Jacob Chapman

#include <string>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Generic_CSV.h"

Appliance_Generic_CSV::Appliance_Generic_CSV() {}

/**
 * @brief read in the csv file containing the power values
 */
void Appliance_Generic_CSV::setup() {
  enableSupply = false;
  enableDemand = false;
  if (fileSupply != "") {
    enableSupply = true;
    modelSupply.parseConfiguration(SimulationConfig::RunLocation + fileSupply);
  }
  if (fileDemand != "") {
    enableDemand = true;
    modelDemand.parseConfiguration(SimulationConfig::RunLocation + fileDemand);
  }
}

/**
 * @brief retrieves power values for each timestep
 */
void Appliance_Generic_CSV::preprocess() {
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
    int hourOfDay = hour % 24;
    int day = hour / 24;
    int now = numberOfSeconds;
    int end = numberOfSeconds + leninsec;
    double s = 0;
    double d = 0;
    while (now < end) {
      if (enableSupply) {
        s += modelSupply.power(day, minuteOfDay);
      }
      if (enableDemand) {
        d += modelDemand.power(day, minuteOfDay);
      }
      minuteOfDay += 1;
      if (minuteOfDay >= 1440) {
        minuteOfDay = 0;
        day +=1;
      }
      now += 60;
    }
    // watt hour per timeperhour
    supply.push_back(s);
    if (hourlyCost.size() == 24) {
      supplyCost.push_back(hourlyCost[hourOfDay]);
    } else {
      supplyCost.push_back(hourlyCost[0]);
    }
    power.push_back(d);
  }
}

/**
 * @brief Sets the csv file to be read in.
 * @param filename: the file name
 */
void Appliance_Generic_CSV::setFileSupply(const std::string & filename) {
  this->fileSupply = filename;
}

/**
 * @brief Sets the csv file to be read in.
 * @param filename: the file name
 */
void Appliance_Generic_CSV::setFileDemand(const std::string & filename) {
  this->fileDemand = filename;
}
