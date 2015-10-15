#include <iostream>
#include "StateMachine.h"

StateMachine::StateMachine()
{
    //ctor
}

unsigned int StateMachine::numberOfStates() const{
	return states.size();
}

void StateMachine::addState(State s)
{
	states.push_back(s);
}

State StateMachine::transistionTo(const int stateID){
    State x;
    for(auto s : states){
        if(s.getId() == stateID){
            x = s;
        }else if(s.hasState( stateID)){
            x = s.getState( stateID);
        }
    }
    return x;
}
