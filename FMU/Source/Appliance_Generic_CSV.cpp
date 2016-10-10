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

void Appliance_Generic_CSV::step() {
  int timeStep = SimulationConfig::getStepCount();
  int leninsec = SimulationConfig::lengthOfTimestep();
  int numberOfSeconds = timeStep * leninsec;
  int minute = numberOfSeconds / 60;
  int minuteOfDay = minute % 1440;
  int hour = numberOfSeconds / 3600;
  int day = hour / 24;
  int now = numberOfSeconds;
  int end = numberOfSeconds + leninsec;
  supply = 0.0;
  power = 0.0;
  while (now < end) {
    if (enableSupply) {
      supply += modelSupply.power(day, minuteOfDay);
    }
    if (enableDemand) {
      power += modelDemand.power(day, minuteOfDay);
    }
    minuteOfDay += 1;
    if (minuteOfDay >= 1440) {
      minuteOfDay = 0;
      day +=1;
    }
    now += 60;
  }

  if (hourlyCost.size() == 24) {
    int hourOfDay = hour % 24;
    supplyCost = hourlyCost[hourOfDay];
  } else if (hourlyCost.size() == 48) {
    int halfHourOfDay = (numberOfSeconds / 1800) % 48;
    supplyCost = hourlyCost[halfHourOfDay];
  } else {
    supplyCost = hourlyCost[0];
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
