#include <iostream>
#include "StateMachine.h"

StateMachine::StateMachine() {}

unsigned int StateMachine::numberOfStates() const{
	return states.size();
}

void StateMachine::addState(const State & s) {
	states.push_back(s);
}

State StateMachine::transistionTo(const int stateID) {
	State x;
	for(const State & s : states){
	  if(s.getId() == stateID){
      x = s;
	  }else if(s.hasState(stateID)) {
      x = s.getState(stateID);
	  }
	}
	return x;
}
