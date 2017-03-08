#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <unordered_map>
#include "State.h"

class StateMachine
{
    public:
      StateMachine();
      void addState(const State & s);
      State transistionTo(const int stateID) const;
      unsigned int numberOfStates() const;
      bool hasState(const int stateID) const;
    private:
      std::unordered_map<int, State> states;

};

#endif // STATEMACHINE_H
