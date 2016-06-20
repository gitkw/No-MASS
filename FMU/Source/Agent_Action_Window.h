#ifndef AGENT_ACTION_WINDOW_H
#define AGENT_ACTION_WINDOW_H

#include "Agent_Action.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Agent_Action_Window : public Agent_Action {
    public:
        Agent_Action_Window();
        void setup(int windowID, int id);
        void setDailyMeanTemperature(double dailyMeanTemperature);
        void saveResult();
        int durationOpen() const;

    protected:

        Model_Windows m_window;
        std::string variableNameWindowDesire;
        double dailyMeanTemperature;

    private:


};

#endif // AGENT_ACTION_WINDOW_H
