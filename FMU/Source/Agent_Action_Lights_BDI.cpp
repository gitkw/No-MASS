// Copyright 2015 Jacob Chapman

#include <vector>
#include "Utility.h"
#include "Agent_Action_Lights_BDI.h"

Agent_Action_Lights_BDI::Agent_Action_Lights_BDI() {
  OffDuringAudioVisual = 0.0;
  OffDuringSleep = 0.0;
}

void Agent_Action_Lights_BDI::setOffDuringAudioVisual(
                                      double OffDuringAudioVisual) {
  this->OffDuringAudioVisual = OffDuringAudioVisual;
}

void Agent_Action_Lights_BDI::setOffDuringSleep(double OffDuringSleep) {
  this->OffDuringSleep = OffDuringSleep;
}

bool Agent_Action_Lights_BDI::doRecipe(const std::vector<double> &activities) {
  bool bdi = false;
  int stepCount = SimulationConfig::getStepCount();
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
