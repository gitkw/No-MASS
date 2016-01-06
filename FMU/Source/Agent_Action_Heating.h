#ifndef AGENT_ACTION_HEATING_H
#define AGENT_ACTION_HEATING_H

#include "Agent_Action.h"

class Agent_Action_Heating : public Agent_Action
{
    public:
        Agent_Action_Heating();
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);

    private:
      
};

#endif // AGENT_ACTION_HEATING_H
