// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_AGENT_ACTION_LIGHTS_H_
#define FMU_SOURCE_AGENT_ACTION_LIGHTS_H_

#include <vector>
#include "Agent_Action.h"

class Agent_Action_Lights : public Agent_Action {
 public:
    Agent_Action_Lights();
    void step(const Building_Zone& zone, const bool inZone,
              const bool previouslyInZone,
              const std::vector<double> &activities);
    void setOffDuringSleep(bool OffDuringSleep);
    void setOffDuringAudioVisual(bool OffDuringAudioVisual);
    bool BDI(const std::vector<double> &activities);

 private:
    bool OffDuringSleep;
    bool OffDuringAudioVisual;
};

#endif  // FMU_SOURCE_AGENT_ACTION_LIGHTS_H_
