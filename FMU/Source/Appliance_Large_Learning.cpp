// Copyright 2016 Jacob Chapman

#include <string>
#include <vector>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"
#include "Appliance_Large_Learning.h"

Appliance_Large_Learning::Appliance_Large_Learning() {}

void Appliance_Large_Learning::setup() {
  std::string buildingString = "Building" + std::to_string(buildingID);
  std::string s_fullname = buildingString + "_Appliance";
  s_fullname = s_fullname + std::to_string(id) + "_";
  databaseIDactual = DataStore::addVariable(s_fullname + "actual");
  qLearning.setEpsilon(epsilon);
  qLearning.setAlpha(alpha);
  qLearning.setGamma(gamma);
  qLearning.setUpdate(update);
  qLearning.setFilename(s_fullname);
  qLearning.setStates(24);
  qLearning.setActions(24);
  qLearning.setId(id);
  qLearning.setup();
  setupModel();
}

/**
 * @brief using Q-learning calculate the learnt start time
 * @details set the current state for Q-learning
 * get the corresponding best action (start time)
 */
void Appliance_Large_Learning::calculateLearntStartTime(
                                              const int hourOfTheDay) {
  qLearning.setState(hourOfTheDay);
  powerProfile.front().startTime = qLearning.getAction();
}

/**
 * @brief Check large appliance model for a turn on, then generate the profile
 * @details Calculate if the applaince is predicted a turn on
 * if so increment model and save teh power demand until turn off
 */
void Appliance_Large_Learning::calculateProfile() {
  int stepCount = SimulationConfig::getStepCount();
  double p = model.consumption(stepCount);
  profileStruct profile;
  while (isOn()) {
    profile.power.push_back(p);
    stepCount++;
    profile.nonLearningStep++;
    model.decreaseDuration();
    p = model.consumption(stepCount);
  }
  if (profile.power.size()) {
    powerProfile.push_back(profile);
  }
}


void Appliance_Large_Learning::startLearningPeriod(const int hourOfTheDay) {
  if (hourOfTheDay == powerProfile.front().startTime &&
    powerProfile.front().isLearningPeriod == false) {
    powerProfile.front().isLearningPeriod = true;
  }
}

/**
 * @brief The reward function
 * @details calculate the reward function
 * @return the reward
 */
double Appliance_Large_Learning::calculateReward() {
  // return -(cost / powerProfile.size() * hoursWaited * 0.1);
  double costOfProfile = powerProfile.front().cost;
  double sizeOfProfile = powerProfile.front().power.size();
  double highestTimeRequired = powerProfile.front().maxTimeRequired;
  return -(costOfProfile / sizeOfProfile * highestTimeRequired);
}

void Appliance_Large_Learning::stopLearningPeriod(const int hourOfTheDay) {
  if (powerProfile.front().learningStep >= powerProfile.front().power.size()) {
    double reward = calculateReward();
    qLearning.setReward(reward);
    qLearning.setState(hourOfTheDay);
    qLearning.learn();
    powerProfile.erase(powerProfile.begin());
  }
}

double Appliance_Large_Learning::getRequiredTime() const {
  int timeStep = SimulationConfig::getStepCount();
  int leninsec = SimulationConfig::lengthOfTimestep();
  int numberOfSeconds = timeStep * leninsec;
  int hour = numberOfSeconds / 3600;
  int hourOfDay = hour % 24;
  return houlyTimeRequired[hourOfDay];
}

void Appliance_Large_Learning::step() {
  setPower(0.0);

  if (powerProfile.empty() == false) {
    int hourOfTheDay = calculateHourOfDay();
    if (powerProfile.front().startTime < 0) {
      calculateLearntStartTime(hourOfTheDay);
    }
    double requiredTime = getRequiredTime();
    if (powerProfile.front().maxTimeRequired < requiredTime) {
      powerProfile.front().maxTimeRequired = requiredTime;
    }
    startLearningPeriod(hourOfTheDay);
    if (powerProfile.front().isLearningPeriod) {
      setPower(powerProfile.front().power[powerProfile.front().learningStep]);
      powerProfile.front().learningStep++;
      stopLearningPeriod(hourOfTheDay);
    }
  }

  stepApplianceOffAndNotLearning();
  saveActualProfile();
}

void Appliance_Large_Learning::saveActualProfile() {
  double p = 0.0;
  if (powerProfile.empty() == false &&
              powerProfile.front().nonLearningStep > 0) {
    int i = powerProfile.front().power.size() -
                                  powerProfile.front().nonLearningStep;
    p = powerProfile.front().power[i];
    powerProfile.front().nonLearningStep--;
  }
  DataStore::addValue(databaseIDactual, p);
}

/**
 * @brief Appliance is off and we are not learning
 * @details check if the appliance model should checked for a turn on
 * get the profile
 * if there is data in the profile get the learn start time
 */
void Appliance_Large_Learning::stepApplianceOffAndNotLearning() {
  if (isOn() || match) {
    calculateProfile();
  }
}

/**
 * @brief Appliance cost is added to for calculating the reward function
 * @param cost cost of using the appliance
 */
void Appliance_Large_Learning::addToCost(const double cost) {
  if (!powerProfile.empty() && powerProfile.front().isLearningPeriod) {
    powerProfile.front().cost += cost;
  }
}

/**
 * @brief Appliance prostprocess, calls Q-Learning save data
 */
void Appliance_Large_Learning::postprocess() {
  qLearning.printQ();
}

void Appliance_Large_Learning::setEpsilon(double epsilon) {
  this->epsilon = epsilon;
}
void Appliance_Large_Learning::setAlpha(double alpha) {
  this->alpha = alpha;
}
void Appliance_Large_Learning::setGamma(double gamma) {
  this->gamma = gamma;
}
void Appliance_Large_Learning::setUpdate(bool update) {
  this->update = update;
}

void Appliance_Large_Learning::setHoulyTimeRequired(
              const std::vector<double> & houlyTimeRequired) {
  this->houlyTimeRequired = houlyTimeRequired;
}
