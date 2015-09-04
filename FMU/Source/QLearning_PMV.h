#ifndef QLEARNING_PMV_H
#define QLEARNING_PMV_H

#include <Agent.h>
#include <QLearning.h>


class QLearning_PMV : public QLearning
{
    public:
        QLearning_PMV();

        void learn(const Zone &zone, struct ActionValues *interaction);
    protected:
    private:



};

#endif // QLEARNING_PMV_H
