#ifndef AGENT_ACTION_WINDOW_STOCHASTIC_H
#define AGENT_ACTION_WINDOW_STOCHASTIC_H

#include "Agent_Action_Window.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Agent_Action_Window_Stochastic : public Agent_Action_Window {
    public:
        Agent_Action_Window_Stochastic();
        void setup(int windowID, int id);
        void step(const Building_Zone& zone, const bool inZone,
                  const bool previouslyInZone,
                  const std::vector<double> &activities);
};

#endif // AGENT_ACTION_WINDOW_STOCHASTIC_H
