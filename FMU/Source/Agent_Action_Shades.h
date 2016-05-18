// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_AGENT_ACTION_SHADES_H_
#define FMU_SOURCE_AGENT_ACTION_SHADES_H_

#include <vector>
#include "Agent_Action.h"
#include "Model_ExternalShading.h"

class Agent_Action_Shades : public Agent_Action {
 public:
  Agent_Action_Shades();

  void setup(int windowID);
  void step(const Building_Zone& zone, const bool inZone,
            const bool previouslyInZone);

  void setClosedDuringSleep(bool ShadeClosedDuringSleep);
  void setClosedDuringWashing(bool ShadeClosedDuringWashing);
  void setClosedDuringNight(bool ShadeClosedDuringNight);
  void setClosedDuringAudioVisual(bool ShadeClosedDuringAudioVisual);

  bool BDI(const std::vector<double> &activities);
 private:
  Model_ExternalShading m_blindUsage;
  bool ShadeClosedDuringSleep;
  bool ShadeClosedDuringWashing;
  bool ShadeClosedDuringNight;
  bool ShadeClosedDuringAudioVisual;
};

#endif  // FMU_SOURCE_AGENT_ACTION_SHADES_H_
