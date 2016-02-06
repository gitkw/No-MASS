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
        virtual void setId(int id);
        void setState(int state);
        void setReward(double reward);
        void setEpsilon(double epsilon);
        void setup();
        virtual double learn(const Zone &zone);
        virtual void reset();
    protected:

        int id;
        int states = 0;
        int actions = 0;
        int state;
        int previous_state;
        int action;
        bool learnNext = false;
        double reward;
        double previous_reward;
    private:
        std::vector<std::vector<double>> qTable;
        double epsilon = 1;   // probability of a random action selection
        double alpha = 0.1;     // learning rate
        double gamma = 0.2;     // discount factor (how soon do you care)


        int getBestAction(int s);


};

#endif // QLEARNING_H
