// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_AGENT_ACTION_LIGHTS_BDI_H_
#define FMU_SOURCE_AGENT_ACTION_LIGHTS_BDI_H_

#include <vector>
#include "Agent_Action_Lights.h"

class Agent_Action_Lights_BDI : public Agent_Action_Lights {
 public:
    Agent_Action_Lights_BDI();

    void setOffDuringSleep(double OffDuringSleep);
    void setOffDuringAudioVisual(double OffDuringAudioVisual);
    bool doRecipe(const std::vector<double> &activities);

 private:
    double OffDuringSleep;
    double OffDuringAudioVisual;
};

#endif  // FMU_SOURCE_AGENT_ACTION_LIGHTS_BDI_H_
