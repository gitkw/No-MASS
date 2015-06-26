#ifndef AGENT_ACTION_HEAT_GAINS_H
#define AGENT_ACTION_HEAT_GAINS_H

#include "Agent_Action.h"


class Agent_Action_Heat_Gains : public Agent_Action
{
    public:

        Agent_Action_Heat_Gains();
        void setup(int agentid);
        void prestep(int id, double clo, double metabolicRate);
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);
        int getPMV();

    protected:
    private:

        int ppd;
        int pmv;
        int id;
        double clo;
        double metabolicRate;
        std::string idAsString;
};

#endif // AGENT_ACTION_HEAT_GAINS_H
