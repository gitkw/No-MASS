#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <unordered_map>
#include "State.hpp"

/**
* @brief Moves an agent into a different state
* @details Moves an agent into a different state
*/

class StateMachine {
  public:
    StateMachine();
    void addState(const State & s);
    bool hasState(const int stateID) const;
    unsigned int numberOfStates() const;
    State transistionTo(const int stateID) const;

  private:
    std::unordered_map<int, State> states;
};

#endif // STATEMACHINE_H
