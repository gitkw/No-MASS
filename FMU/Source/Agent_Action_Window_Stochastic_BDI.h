#ifndef AGENT_ACTION_WINDOW_STOCHASTIC_BDI_H
#define AGENT_ACTION_WINDOW_STOCHASTIC_BDI_H

#include "Agent_Action_Window_Stochastic.h"

class Agent_Action_Window_Stochastic_BDI : public Agent_Action_Window_Stochastic {
    public:
        Agent_Action_Window_Stochastic_BDI();

        void setOpenDuringCooking(double OpenDuringCooking);
        void setOpenDuringWashing(double OpenDuringWashing);
        void setOpenDuringSleeping(double OpenDuringSleeping);
        void setDailyMeanTemperature(double dailyMeanTemperature);
        bool doRecipe(const std::vector<double> &activities);
        bool enabled() const;


    private:
        double OpenDuringWashing;
        double OpenDuringCooking;
        double OpenDuringSleeping;
        double dailyMeanTemperature;

};

#endif // AGENT_ACTION_WINDOW_STOCHASTIC_BDI_H
