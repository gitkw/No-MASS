#ifndef Occupant_ACTION_HEAT_GAINS_H
#define Occupant_ACTION_HEAT_GAINS_H

#include "Occupant_Action.h"


class Occupant_Action_Heat_Gains : public Occupant_Action
{
    public:
        Occupant_Action_Heat_Gains();
        void prestep(double clo, double metabolicRate);
        void step(const Building_Zone& zone, const bool inZone);
        void setup(int agentid);
        double getPMV() const;
        double getPPD() const;

    private:
        int id;
        double clo;
        double metabolicRate;
        double ppd;
        double pmv;
        std::string idAsString;
};

#endif // Occupant_ACTION_HEAT_GAINS_H
