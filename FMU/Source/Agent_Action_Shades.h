#ifndef AGENT_ACTION_SHADES_H
#define AGENT_ACTION_SHADES_H

#include "Agent_Action.h"
#include "Model_ExternalShading.h"

class Agent_Action_Shades : public Agent_Action
{
    public:
        Agent_Action_Shades();

        void setup(int windowID);
        void step(const Building_Zone& zone, const bool inZone,
                  const bool previouslyInZone,
                  const std::vector<double> &activities);

        void setClosedDuringSleep(bool ShadeClosedDuringSleep);
        void setClosedDuringWashing(bool ShadeClosedDuringWashing);
        void setClosedDuringNight(bool ShadeClosedDuringNight);
        void setClosedDuringAudioVisual(bool ShadeClosedDuringAudioVisual);

    private:
        Model_ExternalShading m_blindUsage;
        bool ShadeClosedDuringSleep;
        bool ShadeClosedDuringWashing;
        bool ShadeClosedDuringNight;
        bool ShadeClosedDuringAudioVisual;
};

#endif // AGENT_ACTION_SHADES_H
