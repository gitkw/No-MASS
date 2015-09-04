#ifndef QLEARNING_H
#define QLEARNING_H

#include <vector>
#include "Zone.h"

class QLearning
{
    public:
        QLearning();
        int greedySelection(int s);
        void updateQ(int s, int a, double r, int sp);
        void printQ();
        void setId(int id);
        void setState(int state);
        void setReward(int reward);
        void setup();
        virtual void learn(const Zone &zone, struct ActionValues *act );
    protected:

        int states = 0;
        int actions = 0;
        bool learnNext = false;
        int reward;
        int previous_reward;
        int state;
        int previous_state;
        int action;
    private:
        std::vector<std::vector<double>> qTable;
        double epsilon = 8.0;   // probability of a random action selection
        double alpha = 0.3;     // learning rate
        double gamma = 1.0;     // discount factor
        int id;

        int getBestAction(int s);


};

#endif // QLEARNING_H
