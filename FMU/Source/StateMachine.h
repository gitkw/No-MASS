#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>
#include "State.h"

class StateMachine
{
    public:
      StateMachine();
      void addState(State s);
      State transistionTo(const int stateID);
      unsigned int numberOfStates() const;

    private:
      int initialStateLocation;
      State pendingState;
      std::vector<State> states;

};

#endif // STATEMACHINE_H
