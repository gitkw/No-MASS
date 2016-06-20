// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include "SimulationConfig.h"
#include "Utility.h"
#include "DataStore.h"
#include "Agent_Action_Window_Stochastic_BDI.h"

Agent_Action_Window_Stochastic_BDI::Agent_Action_Window_Stochastic_BDI() {
  OpenDuringWashing = 0.0;
  OpenDuringCooking = 0.0;
  OpenDuringSleeping = 0.0;
}

void Agent_Action_Window_Stochastic_BDI::setOpenDuringWashing(
    const double OpenDuringWashing) {
    this->OpenDuringWashing = OpenDuringWashing;
}

void Agent_Action_Window_Stochastic_BDI::setOpenDuringCooking(
    const double OpenDuringCooking) {
    this->OpenDuringCooking = OpenDuringCooking;
}

void Agent_Action_Window_Stochastic_BDI::setOpenDuringSleeping(
                                      double OpenDuringSleeping) {
  this->OpenDuringSleeping = OpenDuringSleeping;
}

void Agent_Action_Window_Stochastic_BDI::setDailyMeanTemperature(
                                      double dailyMeanTemperature) {
  this->dailyMeanTemperature = dailyMeanTemperature;
}

bool Agent_Action_Window_Stochastic_BDI::doRecipe(
                                const std::vector<double> &activities) {
  bool bdi = false;

  int stepCount = SimulationConfig::getStepCount();
  if (OpenDuringWashing > Utility::randomDouble(0, 1)) {
    if (stepCount > 0) {
      if (activities.at(stepCount - 1) == 6 && activities.at(stepCount) != 6) {
        if (m_window.getWindowState() == 0) {
          m_window.setWindowState(1);
          int lengthOfTimeStepSeconds =
              (60 * (60 / SimulationConfig::info.timeStepsPerHour));
          m_window.setDurationOpen(lengthOfTimeStepSeconds);
          bdi = true;
          result = m_window.getWindowState();
        }
      } else if (stepCount > 1) {
        if (activities.at(stepCount - 2) == 6 &&
            activities.at(stepCount -1) != 6) {
          m_window.setWindowState(0);
          bdi = true;
          result = m_window.getWindowState();
        }
      }
    }
  }

  if (OpenDuringCooking > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 4) {
    if (m_window.getWindowState() == 0) {
      m_window.setWindowState(1);
      int lengthOfTimeStepSeconds =
          (60 * (60 / SimulationConfig::info.timeStepsPerHour));
      m_window.setDurationOpen(lengthOfTimeStepSeconds);
      result = m_window.getWindowState();
      bdi = true;
    }
  }

  if (OpenDuringSleeping > Utility::randomDouble(0, 1)) {
    if (stepCount > 0) {
      if (activities.at(stepCount - 1) == 0 && activities.at(stepCount) != 0) {
        if (m_window.getWindowState() == 0) {
          m_window.setWindowState(1);
          int lengthOfTimeStepSeconds =
              (60 * (60 / SimulationConfig::info.timeStepsPerHour));
          m_window.setDurationOpen(lengthOfTimeStepSeconds);
          bdi = true;
          result = m_window.getWindowState();
        }
      } else if (stepCount > 1) {
        if (activities.at(stepCount - 2) == 0 &&
            activities.at(stepCount -1) != 0) {
          m_window.setWindowState(0);
          bdi = true;
          result = m_window.getWindowState();
        }
      }
    }
  }
  return bdi;
}
