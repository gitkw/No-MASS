// Copyright 2015 Jacob Chapman

#include <iostream>
#include <vector>
#include "Utility.h"
#include "DataStore.h"
#include "Occupant_Action_Shades_BDI.h"
#include "SimulationConfig.h"

Occupant_Action_Shades_BDI::Occupant_Action_Shades_BDI() {
  ShadeClosedDuringSleep = 0.0;
  ShadeClosedDuringWashing = 0.0;
  ShadeClosedDuringNight = 0.0;
  ShadeClosedDuringAudioVisual = 0.0;
}

void Occupant_Action_Shades_BDI::setClosedDuringWashing(
  double ShadeClosedDuringWashing) {
  this->ShadeClosedDuringWashing = ShadeClosedDuringWashing;
}

void Occupant_Action_Shades_BDI::setClosedDuringSleep(
                                  double ShadeClosedDuringSleep) {
  this->ShadeClosedDuringSleep = ShadeClosedDuringSleep;
}

void Occupant_Action_Shades_BDI::setClosedDuringNight(
                                  double ShadeClosedDuringNight) {
  this->ShadeClosedDuringNight = ShadeClosedDuringNight;
}
void Occupant_Action_Shades_BDI::setClosedDuringAudioVisual(
  double ShadeClosedDuringAudioVisual) {
  this->ShadeClosedDuringAudioVisual = ShadeClosedDuringAudioVisual;
}

bool Occupant_Action_Shades_BDI::doRecipe(const std::vector<double> &activities) {
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
      if (Lumint < 50) {
        result = 0;
        bdi = true;
      }
  }
  return bdi;
}
