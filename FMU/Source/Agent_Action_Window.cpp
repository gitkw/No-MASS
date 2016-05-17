// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"

Agent_Action_Window::Agent_Action_Window() {
  OpenDuringWashing = false;
  OpenDuringCooking = false;
  OpenDuringSleeping = false;
}

void Agent_Action_Window::setOpenDuringWashing(
    const bool OpenDuringWashing) {
    this->OpenDuringWashing = OpenDuringWashing;
}

void Agent_Action_Window::setOpenDuringCooking(
    const bool OpenDuringCooking) {
    this->OpenDuringCooking = OpenDuringCooking;
}

void Agent_Action_Window::setOpenDuringSleeping(bool OpenDuringSleeping){
  this->OpenDuringSleeping = OpenDuringSleeping;
}

void Agent_Action_Window::setDailyMeanTemperature(double dailyMeanTemperature){
  this->dailyMeanTemperature = dailyMeanTemperature;
}

void Agent_Action_Window::setup(int windowID, int id) {
  windowStruct ws = SimulationConfig::windows.at(windowID);
  m_window.setDurationVars(ws.aop, ws.bopout, ws.shapeop);
  m_window.setArrivalVars(ws.a01arr, ws.b01inarr, ws.b01outarr,
      ws.b01absprevarr, ws.b01rnarr);
  m_window.setInterVars(ws.a01int, ws.b01inint, ws.b01outint,
      ws.b01presint, ws.b01rnint);
  m_window.setDepartureVars(ws.a01dep, ws.b01outdep, ws.b01absdep,
      ws.b01gddep, ws.a10dep, ws.b10indep, ws.b10outdep, ws.b10absdep,
      ws.b10gddep);

  variableNameWindowDesire = "Agent_Window_Desire" + std::to_string(id);
  DataStore::addVariable(variableNameWindowDesire);
}

void Agent_Action_Window::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone, const std::vector<double> &activities) {
  double outdoorTemperature =
    DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");

  // double rain = DataStore::getValue("EnvironmentSiteRainStatus");
  double rain = 0;
  double indoorTemperature = zone.getMeanAirTemperature();
  double timeStepLengthInMinutes = SimulationConfig::lengthOfTimestep() / 60;

  m_window.setWindowState(zone.getWindowState());
  if(m_window.getWindowState() == 0){
    m_window.setDurationOpen(0);
  }
  if (inZone && !previouslyInZone) {    
    double previousDuration = getPreviousDurationOfAbsenceState(activities);
    m_window.arrival(indoorTemperature,
        outdoorTemperature, previousDuration, rain, timeStepLengthInMinutes);
  } else if (inZone && previouslyInZone) {
    double currentDuration = getCurrentDurationOfPresenceState(activities);
    m_window.intermediate(indoorTemperature,
        outdoorTemperature, currentDuration, rain, timeStepLengthInMinutes);
  } else if (!inZone && previouslyInZone) {
    double groundFloor = zone.getGroundFloor();
    double futureDuration = getFutureDurationOfAbsenceState(activities);
    m_window.departure(
        indoorTemperature, dailyMeanTemperature, futureDuration, groundFloor);
  }
   result = m_window.getWindowState();
}

void Agent_Action_Window::saveResult() {
  DataStore::addValue(variableNameWindowDesire, result);
}



bool Agent_Action_Window::BDI(const std::vector<double> &activities) {
  bool bdi = false;

  int stepCount = SimulationConfig::getStepCount();
  if (OpenDuringWashing){
    if (stepCount > 0 ){
      if (activities.at(stepCount - 1 ) == 6 && activities.at(stepCount) != 6) {
        if (m_window.getWindowState() == 0) {
          m_window.setWindowState(1);
          int lengthOfTimeStepSeconds =
              (60 * (60 / SimulationConfig::info.timeStepsPerHour));
          m_window.setDurationOpen(lengthOfTimeStepSeconds);
          bdi = true;
          result = m_window.getWindowState();
        }
      } else if (stepCount > 1) {
        if (activities.at(stepCount - 2 ) == 6 && activities.at(stepCount -1) != 6) {
          m_window.setWindowState(0);
          bdi = true;
          result = m_window.getWindowState();
        }
      }
    }
  }

  if (OpenDuringCooking && activities.at(stepCount) == 4) {
    if (m_window.getWindowState() == 0) {
      m_window.setWindowState(1);
      int lengthOfTimeStepSeconds =
          (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      m_window.setDurationOpen(lengthOfTimeStepSeconds);
      result = m_window.getWindowState();
      bdi = true;
    }
  }

  if (OpenDuringSleeping){
    if (stepCount > 0 ){
      if (activities.at(stepCount - 1 ) == 0 && activities.at(stepCount) != 0) {
        if (m_window.getWindowState() == 0) {
          m_window.setWindowState(1);
          int lengthOfTimeStepSeconds =
              (60 * (60 / SimulationConfig::info.timeStepsPerHour));
          m_window.setDurationOpen(lengthOfTimeStepSeconds);
          bdi = true;
          result = m_window.getWindowState();
        }
      } else if (stepCount > 1) {
        if (activities.at(stepCount - 2 ) == 0 && activities.at(stepCount -1) != 0) {
          m_window.setWindowState(0);
          bdi = true;
          result = m_window.getWindowState();
        }
      }
    }
  }


  return bdi;
}


int Agent_Action_Window::durationOpen() const{
    return m_window.getDurationOpen();
}
