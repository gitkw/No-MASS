#ifndef RLEARNING_H
#define RLEARNING_H

#include <vector>
#include "Agent.h"

class QLearning
{
    public:
        QLearning();
        int greedySelection(int s);
        void updateQ(int s, int a, double r, int sp);
        void printQ();
        void setId(int id);
        void setup();
        virtual void learn(const Zone &zone, struct ActionValues *act );
    protected:

        int states = 0;
        int actions = 0;
        bool learnNext = false;
    private:
        std::vector<std::vector<double>> qTable;
        double epsilon = 8.0;   // probability of a random action selection
        double alpha = 0.3;     // learning rate
        double gamma = 1.0;     // discount factor
        int id;

        int getBestAction(int s);
};

#endif // RLEARNING_H
