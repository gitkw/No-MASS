#ifndef AGENT_ACTION_WINDOW_LEARNING_H
#define AGENT_ACTION_WINDOW_LEARNING_H

#include <memory>
#include "QLearning.h"
#include "Agent_Action_Window.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Agent_Action_Window_Learning : public Agent_Action_Window {
    public:

        void setup(const int id, const int learn);
        void step(const Building_Zone& zone, const bool inZone);
        void print();
        void reset();
    private:

        
        std::shared_ptr<QLearning> learn;
        std::string window_name;
};

#endif // AGENT_ACTION_WINDOW_LEARNING_H
