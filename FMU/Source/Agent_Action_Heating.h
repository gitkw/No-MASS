#ifndef AGENT_ACTION_HEATING_H
#define AGENT_ACTION_HEATING_H

#include "Agent_Action.h"

class Agent_Action_Heating : public Agent_Action
{
    public:
        Agent_Action_Heating();
        void step(const std::vector<double> &activities);
};

#endif // AGENT_ACTION_HEATING_H
