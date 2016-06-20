// Copyright 2016 Jacob Chapman

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "QLearning_Window.h"
#include "Agent_Action_Window_Learning.h"

void Agent_Action_Window_Learning::print() {
  learn.printQ();
}
void Agent_Action_Window_Learning::reset() {
  learn.reset();
}

void Agent_Action_Window_Learning::setup(const int id, const int learnid) {
  variableNameWindowDesire = "Agent_Window_Desire" + std::to_string(learnid);
  DataStore::addVariable(variableNameWindowDesire);
  std::string zoneIdStr = std::to_string(zoneId);
  learn.setFilename("window-" + zoneIdStr + "-");
  //  learn.setStates(630);
  //  learn.setStates(1260);  // double for depatures
  learn.setStates(400);
  learn.setId(learnid);
  learn.setup();
  window_name = "Weekday-" + zoneIdStr + "-_pmv" + std::to_string(learnid);
  DataStore::addVariable(window_name);
  result = 0;
}

void Agent_Action_Window_Learning::step(const Building_Zone& zone,
      const bool inZone, const bool previouslyInZone) {
  if (inZone || previouslyInZone) {
    double pmv = reward;
    int temp = zone.getMeanAirTemperature();
    int outdoorTemperature =
      DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");
    if (temp > 29) temp = 29;
    if (temp < 10) temp = 10;
    temp = temp -10;
    if (outdoorTemperature > 29) outdoorTemperature = 29;
    if (outdoorTemperature < 10) outdoorTemperature = 10;
    outdoorTemperature = outdoorTemperature - 10;

    int state = (temp * 20) + outdoorTemperature;
  //  if (previouslyInZone && !inZone) {
    //  state += 630;
  //  }

    learn.setState(state);
    int winState = zone.getWindowState();
  //  learn.setState(temp);
    reward = 0;
    if (winState == 1 && pmv > 0.5) {
      reward = 0.1;
    }
    if (winState == 0 && pmv > 0.5) {
      reward = -0.1;
    }
    if (winState == 1 && pmv < -0.5) {
      reward = -0.1;
    }
    if (winState == 0 && pmv < -0.5) {
      reward = 0.1;
    }

    DataStore::addValue(window_name, pmv);

    learn.setAction(zone.getWindowState());
    learn.setReward(reward);
    result = learn.learn();
  }
}
