#ifndef AGENT_ACTION_WINDOW_H
#define AGENT_ACTION_WINDOW_H

#include "Agent_Action.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Agent_Action_Window : public Agent_Action {
    public:
        Agent_Action_Window();
        void setup(int windowID, int id);
        void step(const Building_Zone& zone, const bool inZone,
                  const bool previouslyInZone,
                  const std::vector<double> &activities);
        void setOpenDuringCooking(bool OpenDuringCooking);
        void setOpenDuringWashing(bool OpenDuringWashing);
        void setOpenDuringSleeping(bool OpenDuringSleeping);
        void setDailyMeanTemperature(double dailyMeanTemperature);
        void saveResult();

        bool BDI(const std::vector<double> &activities);
    protected:

        Model_Windows m_window;
        std::string variableNameWindowDesire;

    private:
        bool OpenDuringWashing;
        bool OpenDuringCooking;
        bool OpenDuringSleeping;
        double dailyMeanTemperature;
};

#endif // AGENT_ACTION_WINDOW_H
