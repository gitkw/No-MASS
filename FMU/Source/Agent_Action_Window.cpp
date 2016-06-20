// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "Utility.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"

Agent_Action_Window::Agent_Action_Window() {
}

void Agent_Action_Window::setDailyMeanTemperature(double dailyMeanTemperature) {
  this->dailyMeanTemperature = dailyMeanTemperature;
}

void Agent_Action_Window::setup(int windowID, int id) {
  variableNameWindowDesire = "Agent_Window_Desire_"
                            + std::to_string(id) + "_"
                            + std::to_string(windowID);
  DataStore::addVariable(variableNameWindowDesire);
}

void Agent_Action_Window::saveResult() {
  DataStore::addValue(variableNameWindowDesire, result);
}

int Agent_Action_Window::durationOpen() const {
    return m_window.getDurationOpen();
}
