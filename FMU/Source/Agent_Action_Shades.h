#ifndef AGENT_ACTION_SHADES_H
#define AGENT_ACTION_SHADES_H

#include "Agent_Action.h"


class Agent_Action_Shades : public Agent_Action
{
    public:
        Agent_Action_Shades();
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);
    protected:
    private:
};

#endif // AGENT_ACTION_SHADES_H
