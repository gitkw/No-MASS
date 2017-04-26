// Copyright 2016 Jacob Chapman

#include <string>
#include "DataStore.hpp"
#include "Configuration.hpp"
#include "Utility.hpp"
#include "Appliance_Generic_CSV.hpp"

Appliance_Generic_CSV::Appliance_Generic_CSV() {}

/**
 * @brief read in the csv file containing the power values
 */
void Appliance_Generic_CSV::setup(ConfigStructAppliance a) {
  setID(a.id);
  setHoulyPriority(a.priority);
  setHourlyCost(a.costVector);
  setFileDemand(a.fileDemand);
  setFileSupply(a.fileSupply);

  enableSupply = false;
  enableDemand = false;
  if (fileSupply != "") {
    enableSupply = true;
    modelSupply.parseConfiguration(Configuration::RunLocation + fileSupply);
  }
  if (fileDemand != "") {
    enableDemand = true;
    modelDemand.parseConfiguration(Configuration::RunLocation + fileDemand);
  }



}

void Appliance_Generic_CSV::step() {
  int timeStep = Configuration::getStepCount();
  int leninsec = Configuration::lengthOfTimestep();
  int numberOfSeconds = timeStep * leninsec;
  int minute = numberOfSeconds / 60;
  int minuteOfDay = minute % 1440;
  int hour = numberOfSeconds / 3600;
  int day = hour / 24;
  int now = numberOfSeconds;
  int end = numberOfSeconds + leninsec;
  setSupply(0.0);
  setPower(0.0);
  while (now < end) {
    if (enableSupply) {
      setSupply(getSupply() + modelSupply.power(day, minuteOfDay));
      setSupplyLeft(getSupply());
    }
    if (enableDemand) {
      setPower(getPower() + modelDemand.power(day, minuteOfDay));
    }
    minuteOfDay += 1;
    if (minuteOfDay >= 1440) {
      minuteOfDay = 0;
      day +=1;
    }
    now += 60;
  }

  int p = 0;
  if (hourlyCost.size() == 24) {
    p = hour % 24;
  } else if (hourlyCost.size() == 48) {
    p = (numberOfSeconds / 1800) % 48;
  }
  setSupplyCost(hourlyCost[p]);
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
