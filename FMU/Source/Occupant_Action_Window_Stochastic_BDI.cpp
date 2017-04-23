// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include "Configuration.hpp"
#include "Utility.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Window_Stochastic_BDI.hpp"

Occupant_Action_Window_Stochastic_BDI::Occupant_Action_Window_Stochastic_BDI() {
  OpenDuringWashing = 0.0;
  OpenDuringCooking = 0.0;
  OpenDuringSleeping = 0.0;
}

void Occupant_Action_Window_Stochastic_BDI::setOpenDuringWashing(
    const double OpenDuringWashing) {
    this->OpenDuringWashing = OpenDuringWashing;
}

void Occupant_Action_Window_Stochastic_BDI::setOpenDuringCooking(
    const double OpenDuringCooking) {
    this->OpenDuringCooking = OpenDuringCooking;
}

void Occupant_Action_Window_Stochastic_BDI::setOpenDuringSleeping(
                                      double OpenDuringSleeping) {
  this->OpenDuringSleeping = OpenDuringSleeping;
}

void Occupant_Action_Window_Stochastic_BDI::setDailyMeanTemperature(
                                      double dailyMeanTemperature) {
  this->dailyMeanTemperature = dailyMeanTemperature;
}

bool Occupant_Action_Window_Stochastic_BDI::doRecipe(
                                const std::vector<double> &activities) {
  bool bdi = false;

  int stepCount = Configuration::getStepCount();
  if (OpenDuringWashing > Utility::randomDouble(0, 1)) {
    if (stepCount > 0) {
      if (activities.at(stepCount - 1) == 6 && activities.at(stepCount) != 6) {
        if (m_window.getWindowState() == 0) {
          m_window.setWindowState(1);
          int lengthOfTimeStepSeconds =
              (60 * (60 / Configuration::info.timeStepsPerHour));
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
          (60 * (60 / Configuration::info.timeStepsPerHour));
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
              (60 * (60 / Configuration::info.timeStepsPerHour));
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
