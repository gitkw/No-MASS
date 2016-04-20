// Copyright 2015 Jacob Chapman

#include "QLearning_PMV.h"

QLearning_PMV::QLearning_PMV() {
    states = 7;
    actions = 3;
}

void QLearning_PMV::learn(const Zone &zone, ActionValues *act) {
    bool doRLearn = false;
    if (zone.getWindowState() != act->windowState &&
            zone.getLightState() != act->lightState) {
        doRLearn = true;
    } else if (zone.getLightState() != act->lightState &&
            zone.getBlindState() != act->shadeState) {
        doRLearn = true;
    } else if (zone.getBlindState() != act->shadeState &&
            zone.getWindowState() != act->windowState) {
        doRLearn = true;
    }

    if (learnNext) {
        reward = -1;
        if (state == 0) {
            reward = 1;
        }
        // get new state
        // update q table that is learn
        updateQ(state+3, action, reward, previous_state+3);
        learnNext = false;
    }

    if (doRLearn) {
        previous_state = state;
        previous_reward = reward;
        // get action for current state using greedy
        action = greedySelection(state + 3);
        switch (action) {
        case 0  :  // window
            act->lightState = zone.getLightState();
            act->shadeState = zone.getBlindState();

            break;
        case 1  :  // shade
            act->lightState = zone.getLightState();
            act->windowState = zone.getWindowState();

            break;
        case 2  :  // light
            act->shadeState = zone.getBlindState();
            act->windowState = zone.getWindowState();

            break;
        }
        learnNext = true;
    }
}
