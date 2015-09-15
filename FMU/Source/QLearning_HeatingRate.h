#ifndef QLEARNING_HEATINGRATE_H
#define QLEARNING_HEATINGRATE_H

#include "QLearning.h"
#include "Agent.h"

class QLearning_HeatingRate : public QLearning
{
    public:
        QLearning_HeatingRate();

        void learn(const Zone &zone, struct ActionValues *interaction);
    protected:
    private:
};

#endif // QLEARNING_HEATINGRATE_H
