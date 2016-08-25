// Copyright 2016 Jacob Chapman

#include <string>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"
#include "Appliance_Large_Learning.h"

Appliance_Large_Learning::Appliance_Large_Learning() {}

void Appliance_Large_Learning::setup() {
  isLearningPeriod = false;
  isWaitingForApplianceToStart = false;
  startTime = -1;
  nonLearningStep = 0;
  std::string buildingString = "Building" + std::to_string(buildingID);
  std::string s_fullname = buildingString + "_Appliance" + std::to_string(id);
  s_fullname_actual = s_fullname + "_actual";
  DataStore::addVariable(s_fullname_actual);
  qLearning.setEpsilon(epsilon);
  qLearning.setAlpha(alpha);
  qLearning.setGamma(gamma);
  qLearning.setUpdate(update);
  qLearning.setFilename(s_fullname + "-");
  qLearning.setStates(24);
  qLearning.setId(id);
  qLearning.setup();
  setupModel();
}

/**
 * @brief using Q-learning calculate the learnt start time
 * @details set the current state for Q-learning
 * get the corresponding best action (start time)
 */
void Appliance_Large_Learning::calculateLearntStartTime() {
  int hourOfTheDay = calculateHourOfDay();
  qLearning.setState(hourOfTheDay);
  startTime = qLearning.getAction();
  learningStep = 0;
  isWaitingForApplianceToStart = true;
}

/**
 * @brief Check large appliance model for a turn on, then generate the profile
 * @details Calculate if the applaince is predicted a turn on
 * if so increment model and save teh power demand until turn off
 */
void Appliance_Large_Learning::calculateProfile() {
  int stepCount = SimulationConfig::getStepCount();
  double p = model.consumption(stepCount);
  while (isOn()) {
    powerProfile.push_back(p);
    model.decreaseDuration();
    stepCount++;
    nonLearningStep++;
    p = model.consumption(stepCount);
  }
}


void Appliance_Large_Learning::startLearningPeriod(const int hourOfTheDay) {
  if (hourOfTheDay == startTime && isWaitingForApplianceToStart) {
    isWaitingForApplianceToStart = false;
    isLearningPeriod = true;
    startTime = startTime -1;
    cost = 0.0;
  }
}

/**
 * @brief The reward function
 * @details calculate the reward function
 * @return the reward
 */
double Appliance_Large_Learning::calculateReward() {
  return -cost;
}

void Appliance_Large_Learning::stopLearningPeriod(const int hourOfTheDay) {
  if (learningStep >= powerProfile.size()) {
    double reward = calculateReward();
    qLearning.setReward(reward);
    qLearning.setState(hourOfTheDay);
    qLearning.learn();
    powerProfile.clear();
    isLearningPeriod = false;
    startTime = -1;
  }
}

void Appliance_Large_Learning::step() {
  double p = 0.0;
  int hourOfTheDay = calculateHourOfDay();
  startLearningPeriod(hourOfTheDay);
  if (isLearningPeriod) {
    p = powerProfile[learningStep];
    learningStep++;
    stopLearningPeriod(hourOfTheDay);
  } else if (isWaitingForApplianceToStart == false) {
    stepApplianceOffAndNotLearning();
  }
  saveActualProfile();
  power.push_back(p);
  supply.push_back(0.0);
  supplyCost.push_back(0.0);
}

void Appliance_Large_Learning::saveActualProfile() {
  double p = 0.0;
  if (nonLearningStep > 0) {
    int i = powerProfile.size() - nonLearningStep;
    p = powerProfile[i];
    nonLearningStep--;
  }
  DataStore::addValue(s_fullname_actual, p);
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
    if (powerProfile.size() > 0) {
     calculateLearntStartTime();
    }
  }
}

/**
 * @brief Appliance cost is added to for calculating the reward function
 * @param cost cost of using the appliance
 */
void Appliance_Large_Learning::addToCost(const double cost) {
  this->cost += cost;
}

/**
 * @brief Appliance prostprocess, calls Q-Learning save data
 */
void Appliance_Large_Learning::postprocess() {
  qLearning.printQ();
}


int Appliance_Large_Learning::calculateHourOfDay() const {
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  return hour % 24;
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
