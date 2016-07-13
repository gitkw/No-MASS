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
  void setIndoorIlluminance(const float lumint);

 protected:
   float Lumint = 0;

 private:

  Model_ExternalShading m_blindUsage;
};

#endif  // FMU_SOURCE_AGENT_ACTION_SHADES_H_
