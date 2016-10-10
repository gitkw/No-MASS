#ifndef Occupant_ACTION_WINDOW_LEARNING_H
#define Occupant_ACTION_WINDOW_LEARNING_H

#include <memory>
#include "QLearning_Window.h"
#include "Occupant_Action_Window.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Occupant_Action_Window_Learning : public Occupant_Action_Window {
    public:

        void setup(const int id);
        void step(const Building_Zone& zone, const bool inZone,
        const bool previouslyInZone);
        void print();
        void reset();
    private:

        QLearning_Window learn;
        int window_name;
};

#endif // Occupant_ACTION_WINDOW_LEARNING_H
