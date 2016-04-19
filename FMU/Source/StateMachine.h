#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>
#include "State.h"

class StateMachine
{
    public:
      StateMachine();
      void addState(const State & s);
      State transistionTo(const int stateID);
      unsigned int numberOfStates() const;

    private:
      std::vector<State> states;

};

#endif // STATEMACHINE_H
