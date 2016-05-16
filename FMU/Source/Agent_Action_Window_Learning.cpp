// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include <cmath>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "QLearning_Window.h"
#include "Agent_Action_Window_Learning.h"

void Agent_Action_Window_Learning::print() {
  learn->printQ();
}
void Agent_Action_Window_Learning::reset() {
  learn->reset();
}

void Agent_Action_Window_Learning::setup(const int id, const int learnid) {
  variableNameWindowDesire = "Agent_Window_Desire" + std::to_string(learnid);
  DataStore::addVariable(variableNameWindowDesire);
  learn = std::shared_ptr<QLearning_Window>(
  new QLearning_Window());


  std::string zoneIdStr = std::to_string(zoneId);
  learn->setFilename("window-" + zoneIdStr + "-");

  learn->setStates(6);
  learn->setStates(630);

  learn->setStates(21);
  learn->setStates(630);
  learn->setId(learnid);
  learn->setup();
  window_name = "Weekday-" + zoneIdStr + "-_pmv" + std::to_string(learnid);
  DataStore::addVariable(window_name);
  result = 0;
}

void Agent_Action_Window_Learning::step(const Building_Zone& zone, const bool inZone) {
  if (inZone) {
    double pmv = reward;
    int temp = zone.getMeanAirTemperature();
    int outdoorTemperature =
      DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");
    if (temp > 29) temp = 29;
    if (temp < 10) temp = 10;
    temp = temp -10;
    if (outdoorTemperature > 30) outdoorTemperature = 30;
    if (outdoorTemperature < 1) outdoorTemperature = 1;
    outdoorTemperature = outdoorTemperature;

    int state = (temp * 30) + outdoorTemperature;
    learn->setState(state);

  //  learn->setState(temp);
    reward = 0;
    if(result == 1 && pmv > 0.5){
      reward = 0.1;
    }
    if(result == 0 && pmv > 0.5){
      reward = -0.1;
    }
    if(result == 1 && pmv < -0.5){
      reward = -0.1;
    }
    if(result == 0 && pmv < -0.5){
      reward = 0.1;
    }

    DataStore::addValue(window_name, pmv);

    learn->setAction(zone.getWindowState());
    learn->setReward(reward);
    result = learn->learn();
  }
}
