// Copyright 2016 Jacob Chapman

#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Configuration.hpp"
#include "Utility.hpp"
#include "Appliance_Large.hpp"

Appliance_Large::Appliance_Large() {}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 */
void Appliance_Large::setup(ConfigStructAppliance a) {
  setID(a.id);
  setHoulyPriority(a.priority);
  setActivities(a.activities);
  setFile(a.fileProfile);
  setupModel();
}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 * @details Sets the large appliance configuration file and gives the model
 * the id of the appliance in the file
 */
void Appliance_Large::setupModel() {
  model.setID(id);
  model.parseConfiguration(Configuration::FileLargeAppliance);
}

/**
 * @brief The timestep call of the appliance
 * @details If the appliance is on increase the survival time and retrieve
 * the power. \n
 * If the appliance is off calculate if the appliance turns on and how long for
 * and retrieve power. \n
 * Otherwise set power to 0.
 */
void Appliance_Large::step() {
  model.decreaseDuration();
  setPower(0.0);
  int stepCount = Configuration::getStepCount();
  if (model.onAt(stepCount) && match) {
    double p = model.consumption(stepCount);
    setPower(p);
  }
}

/**
 * @brief Check the model for if the appliance is turned on.
 * @return if the appliance is turned on or not.
 */
bool Appliance_Large::isOn() const {
  return model.isOn();
}


void Appliance_Large::setFile(std::string file) {
  this->file = file;
}
