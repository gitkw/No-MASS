#ifndef RLEARNING_H
#define RLEARNING_H


class RLearning
{
    public:
        RLearning();
        int greedySelection(int s);
        void updateQ(int s, int a, double r, int sp);
        void printQ();
        void setId(int id);
        void setup();

    private:

        static const int states = 7;
        static const int actions = 3;
        double qTable[states][actions];
        double epsilon = 8.0;   // probability of a random action selection
        double alpha = 0.3;     // learning rate
        double gamma = 1.0;     // discount factor
        int id;

        int getBestAction(int s);
};

#endif // RLEARNING_H
