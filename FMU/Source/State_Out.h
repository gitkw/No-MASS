#ifndef STATE_OUT_H
#define STATE_OUT_H

#include "./State.h"

class State_Out: public State
{
    public:
        State_Out();
        bool hasZone() const;
};

#endif // STATE_OUT_H
