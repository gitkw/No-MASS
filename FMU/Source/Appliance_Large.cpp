// Copyright 2016 Jacob Chapman

#include <cassert>
#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_Large.h"

Appliance_Large::Appliance_Large() {}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 */
void Appliance_Large::setup() {
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
  model.parseConfiguration(SimulationConfig::FileLargeAppliance);
}

/**
 * @brief The timestep call of the appliance
 * @details If the appliance is on increase the survival time and retrieve
 * the power. \n
 * If the appliance is off calculate if the appliance turns on and how long for
 * and retrieve power. \n
 * Otherwise set power to 0.
 */
/*void Appliance_Large::step() {
  model.decreaseDuration();
  setPower(0.0);
  if (isOn() || match) {
    int stepCount = SimulationConfig::getStepCount();
    setPower(model.consumption(stepCount));
  }
}
*/
void Appliance_Large::step() {
  model.decreaseDuration();
  setPower(0.0);
  int stepCount = SimulationConfig::getStepCount();
  double p = model.consumption(stepCount);
  if (isOn() || match) {
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

/**
 * @brief Check the activities the appliance can be turned on during.
 * @return If there is a corresponding activity to turn the appliance on in.
 * @details For some appliances we may wish that they only turn on for some
 * applainces, here we check if the activities match.
 */
bool Appliance_Large::hasActivities(std::vector<int> Activities) {
  match = this->Activities.empty();
  for (int i : this->Activities) {
    for (int j : Activities) {
      if (i == j) {
          match = true;
          break;
      }
    }
  }
  return match;
}

/**
 * @brief Set the activity for which the appliance can be turned on during.
 * @param Activities: A set of activities
 */
void Appliance_Large::setActivities(const std::vector<int> Activities) {
  this->Activities = Activities;
}
