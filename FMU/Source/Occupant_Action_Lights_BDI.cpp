// Copyright 2015 Jacob Chapman

#include <vector>
#include "Utility.hpp"
#include "Occupant_Action_Lights_BDI.hpp"

Occupant_Action_Lights_BDI::Occupant_Action_Lights_BDI() {
  OffDuringAudioVisual = 0.0;
  OffDuringSleep = 0.0;
}

void Occupant_Action_Lights_BDI::setOffDuringAudioVisual(
                                      double OffDuringAudioVisual) {
  this->OffDuringAudioVisual = OffDuringAudioVisual;
}

void Occupant_Action_Lights_BDI::setOffDuringSleep(double OffDuringSleep) {
  this->OffDuringSleep = OffDuringSleep;
}

bool Occupant_Action_Lights_BDI::doRecipe(const std::vector<double> &activities) {
  bool bdi = false;
  int stepCount = Configuration::getStepCount();
  if (OffDuringSleep > Utility::randomDouble(0, 1) &&
        activities.at(stepCount) == 0) {
      result = 0;
      bdi = true;
  }
  if (OffDuringAudioVisual > Utility::randomDouble(0, 1) &&
        activities.at(stepCount) == 2) {
      result = 0;
      bdi = true;
  }
  return bdi;
}
