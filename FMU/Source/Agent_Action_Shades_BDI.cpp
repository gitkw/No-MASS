// Copyright 2015 Jacob Chapman

#include <iostream>
#include <vector>
#include "Utility.h"
#include "DataStore.h"
#include "Agent_Action_Shades_BDI.h"
#include "SimulationConfig.h"

Agent_Action_Shades_BDI::Agent_Action_Shades_BDI() {
  ShadeClosedDuringSleep = 0.0;
  ShadeClosedDuringWashing = 0.0;
  ShadeClosedDuringNight = 0.0;
  ShadeClosedDuringAudioVisual = 0.0;
}

void Agent_Action_Shades_BDI::setClosedDuringWashing(
  double ShadeClosedDuringWashing) {
  this->ShadeClosedDuringWashing = ShadeClosedDuringWashing;
}

void Agent_Action_Shades_BDI::setClosedDuringSleep(
                                  double ShadeClosedDuringSleep) {
  this->ShadeClosedDuringSleep = ShadeClosedDuringSleep;
}

void Agent_Action_Shades_BDI::setClosedDuringNight(
                                  double ShadeClosedDuringNight) {
  this->ShadeClosedDuringNight = ShadeClosedDuringNight;
}
void Agent_Action_Shades_BDI::setClosedDuringAudioVisual(
  double ShadeClosedDuringAudioVisual) {
  this->ShadeClosedDuringAudioVisual = ShadeClosedDuringAudioVisual;
}

bool Agent_Action_Shades_BDI::doRecipe(const std::vector<double> &activities) {
  bool bdi = false;
  int stepCount = SimulationConfig::getStepCount();
  if (ShadeClosedDuringSleep > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 0) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringWashing > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 6) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringAudioVisual > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 2) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringNight > Utility::randomDouble(0, 1)) {
      int hour = DataStore::getValue("hourOfDay");
      if (hour < 8) {
        result = 0;
        bdi = true;
      } else if (hour > 20) {
        result = 0;
        bdi = true;
      }
  }
  return bdi;
}
