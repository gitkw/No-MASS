#ifndef QLEARNING_HEATINGSETPOINTS_H
#define QLEARNING_HEATINGSETPOINTS_H

#include "Agent.h"
#include "QLearning.h"

class QLearning_HeatingSetPoints : public QLearning
{
    public:
        QLearning_HeatingSetPoints();

        double learn(const Zone &zone);
    protected:
    private:
};

#endif // QLEARNING_HEATINGSETPOINTS_H
