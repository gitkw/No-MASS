// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_Agent_Action_Shades_BDI_H_
#define FMU_SOURCE_Agent_Action_Shades_BDI_H_

#include <vector>
#include "Agent_Action_Shades.h"

class Agent_Action_Shades_BDI : public Agent_Action_Shades {
 public:
  Agent_Action_Shades_BDI();
  void setClosedDuringSleep(double ShadeClosedDuringSleep);
  void setClosedDuringWashing(double ShadeClosedDuringWashing);
  void setClosedDuringNight(double ShadeClosedDuringNight);
  void setClosedDuringAudioVisual(double ShadeClosedDuringAudioVisual);

  bool doRecipe(const std::vector<double> &activities);
 private:

  double ShadeClosedDuringSleep;
  double ShadeClosedDuringWashing;
  double ShadeClosedDuringNight;
  double ShadeClosedDuringAudioVisual;
};

#endif  // FMU_SOURCE_Agent_Action_Shades_BDI_H_
