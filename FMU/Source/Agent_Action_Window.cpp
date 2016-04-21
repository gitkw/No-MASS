// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"

Agent_Action_Window::Agent_Action_Window() {}

void Agent_Action_Window::setOpenDuringWashing(
    const bool OpenDuringWashing) {
    this->OpenDuringWashing = OpenDuringWashing;
}

void Agent_Action_Window::setOpenDuringCooking(
    const bool OpenDuringCooking) {
    this->OpenDuringCooking = OpenDuringCooking;
}

void Agent_Action_Window::setup(int windowID) {
  windowStruct ws = SimulationConfig::windows.at(windowID);
  m_window.setDurationVars(ws.aop, ws.bopout, ws.shapeop);
  m_window.setArrivalVars(ws.a01arr, ws.b01inarr, ws.b01outarr,
      ws.b01absprevarr, ws.b01rnarr);
  m_window.setInterVars(ws.a01int, ws.b01inint, ws.b01outint,
      ws.b01presint, ws.b01rnint);
  m_window.setDepartureVars(ws.a01dep, ws.b01outdep, ws.b01absdep,
      ws.b01gddep, ws.a10dep, ws.b10indep, ws.b10outdep, ws.b10absdep,
      ws.b10gddep);
}

void Agent_Action_Window::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone, const std::vector<double> &activities) {
  double outdoorTemperature =
    DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");

  outDoorTemperatures.push_back(outdoorTemperature);
  if (outDoorTemperatures.size() >
        (SimulationConfig::info.timeStepsPerHour * 24)) {
          outDoorTemperatures.pop_front();
  }

  // double rain = DataStore::getValue("EnvironmentSiteRainStatus");
  double rain = 0;
  double indoorTemperature = zone.getMeanAirTemperature();
  double timeStepLengthInMinutes = SimulationConfig::lengthOfTimestep() / 60;

  m_window.setWindowState(zone.getWindowState());
  if (inZone && !previouslyInZone) {
    double previousDuration = getPreviousDurationOfAbsenceState(activities);
    m_window.arrival(indoorTemperature,
        outdoorTemperature, previousDuration, rain, timeStepLengthInMinutes);
  } else if ((inZone && previouslyInZone )) {
    double currentDuration = getCurrentDurationOfPresenceState(activities);
    m_window.intermediate(indoorTemperature,
        outdoorTemperature, currentDuration, rain, timeStepLengthInMinutes);
  } else if ((!inZone && previouslyInZone )) {
    double dailyMeanTemperature = 0;
    for (double temp : outDoorTemperatures) {
            dailyMeanTemperature += temp;
    }
    dailyMeanTemperature =
      dailyMeanTemperature /
        static_cast<double>(outDoorTemperatures.size());

    double groundFloor = zone.getGroundFloor();
    double futureDuration = getFutureDurationOfAbsenceState(activities);
    m_window.departure(
        indoorTemperature, dailyMeanTemperature, futureDuration, groundFloor);
  }
  int stepCount = SimulationConfig::getStepCount();


  if (OpenDuringCooking && activities.at(stepCount) == 4) {
    if (m_window.getWindowState() == 0) {
      m_window.setWindowState(1);
      int lengthOfTimeStepSeconds =
          (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      m_window.setDurationOpen(lengthOfTimeStepSeconds);
    }
  }

  if (OpenDuringWashing && activities.at(stepCount) == 6) {
    if (m_window.getWindowState() == 0) {
      m_window.setWindowState(1);
      int lengthOfTimeStepSeconds =
          (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      m_window.setDurationOpen(lengthOfTimeStepSeconds);
    }
  }
  result = m_window.getWindowState();
//  if(result)
//  std::cout  << " " << result << " " << m_window.getDurationOpen() << std::endl;

}
