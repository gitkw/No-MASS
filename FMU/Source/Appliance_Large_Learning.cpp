// Copyright 2016 Jacob Chapman

#include <vector>
#include <string>
#include <iostream>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"

#include "Appliance_Large_Learning.h"

Appliance_Large_Learning::Appliance_Large_Learning() {}

void Appliance_Large_Learning::setup() {
  learning = false;
  startTime = -1;
  std::string IdStr = std::to_string(id);
  DataStore::addVariable(IdStr + "_actual");
  qLearning.setFilename("Appliance-" + IdStr + "-");
  qLearning.setStates(24);
  qLearning.setId(id);
  qLearning.setup();
  setupModel();
}

void Appliance_Large_Learning::getStart() {
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  int hourOfTheDay = hour % 24;
  qLearning.setState(hourOfTheDay);
  startTime = qLearning.getAction();
  //std::cout << startTime << " " << hourOfTheDay << std::endl;
  learningStep = 0;
}

void Appliance_Large_Learning::getProfile() {
  int stepCount = SimulationConfig::getStepCount();
  double p = 0.0;
  p = model.consumption(stepCount);
  while (isOn()) {
    p = model.consumption(stepCount);
    powerProfile.push_back(p);
    x++;

    model.decreaseDuration();
    stepCount++;
  }
}

void Appliance_Large_Learning::step() {
  double p = 0.0;
  int stepCount = SimulationConfig::getStepCount();
  int hour = (stepCount * SimulationConfig::lengthOfTimestep()) / 3600;
  int hourOfTheDay = hour % 24;

  if (hourOfTheDay == startTime) {
      startTime = startTime -1;
    learning = true;
    cost = 0.0;
  }
  if (learning) {
    p = powerProfile[learningStep];
    learningStep++;
    std::string IdStr = std::to_string(id);
    if (learningStep >= powerProfile.size()) {
      double reward = -cost;
      //std::cout << startTime << " " << hourOfTheDay << " " << reward << std::endl;
      qLearning.setReward(reward);
      cost = 0.0;
      powerProfile.clear();
      qLearning.setState(hourOfTheDay);
      qLearning.learn();
      learning = false;
      startTime = -1;
    }
  } else if (startTime < 0) {
      stepOff();
  }
  if (x > 0) {
    std::string IdStr = std::to_string(id);
    //std::cout << x << " " << powerProfile.size() - x << " " << powerProfile[powerProfile.size() - x] << std::endl;
    DataStore::addValue(IdStr + "_actual", powerProfile[powerProfile.size() - x]);
    x--;
  }else{
    std::string IdStr = std::to_string(id);
    DataStore::addValue(IdStr + "_actual", 0.0);

  }
  power.push_back(p);
  supply.push_back(0.0);
}

void Appliance_Large_Learning::stepOff() {
  if (isOn() || match) {
    getProfile();
    if (powerProfile.size() > 0) {
     getStart();
    }
  }
}

void Appliance_Large_Learning::addToCost(const double cost) {
  this->cost += cost;
}

void Appliance_Large_Learning::postprocess() {
  qLearning.printQ();
}
