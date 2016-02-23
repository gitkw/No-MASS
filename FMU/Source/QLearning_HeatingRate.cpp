// Copyright 2015 Jacob Chapman
#include "QLearning_HeatingRate.h"

QLearning_HeatingRate::QLearning_HeatingRate() {
    states = 12;
    actions = 2;
}

void QLearning_HeatingRate::learn(const Building_Zone &zone, ActionValues *act ) {
    bool doRLearn = false;
    if (zone.getWindowState() != act->windowState && zone.getLightState() != act->lightState) {
        // std::cout << "window light" << std::endl;
        if (act->windowState && act->lightState) {
            state = 0;

        } else if (act->windowState && !act->lightState) {
            state = 1;

        } else if (!act->windowState && act->lightState) {
            state = 2;

        } else if (!act->windowState && !act->lightState) {
            state = 3;
        }
        doRLearn = true;
    }else if ( zone.getLightState() != act->lightState && zone.getBlindState() != act->shadeState) {
        //  std::cout << "light shade" << std::endl;
        if (act->shadeState > zone.getBlindState()  && act->lightState) {
            state = 4;
        } else if (act->shadeState > zone.getBlindState() && !act->lightState) {
            state = 5;
        } else if (act->shadeState < zone.getBlindState() && act->lightState) {
            state = 6;
        } else if (act->shadeState < zone.getBlindState() && !act->lightState) {
            state = 7;
        }
        doRLearn = true;
    } else if (zone.getBlindState() != act->shadeState && zone.getWindowState() != act->windowState) {
        // std::cout << "shade window" << std::endl;
        if (act->shadeState > zone.getBlindState()  && act->windowState) {
            state = 8;
        } else if (act->shadeState > zone.getBlindState() && !act->windowState) {
            state = 9;
        } else if (act->shadeState < zone.getBlindState() && act->windowState) {
            state = 10;
        } else if (act->shadeState < zone.getBlindState() && !act->windowState) {
            state = 11;
        }
        doRLearn = true;
    }

    if (learnNext) {

        double save_reward = 0;
        if (reward - previous_reward  < 0) {
            for(int i = 1; i <= 20; i ++) {
                if ( previous_reward - reward > i) {
                    save_reward = i;
                }
            }
        }

        // get new state
        // update q table that is learn
        updateQ(state, action, save_reward, previous_state);

        learnNext = false;
    }

    if (doRLearn) {
        previous_state = state;
        previous_reward = reward;
        // get action for current state using greedy
        action = greedySelection(state);
        switch (action) {
        case 0  :  // window
            switch (state) {
                case 0  :  // window
                case 1  :  // window
                case 2  :  // window
                case 3  :  // window
                    act->windowState = zone.getWindowState();
                    break;
                case 4  :  // window
                case 5  :  // window
                case 6  :  // window
                case 7  :  // window
                    act->shadeState = zone.getBlindState();
                    break;
                case 8  :  // window
                case 9  :  // window
                case 10  :  // window
                case 11  :  // window
                    act->windowState = zone.getWindowState();
                    break;
            }
            break;
        case 1  :  // shade
            switch (state) {
                case 0  :  // window
                case 1  :  // window
                case 2  :  // window
                case 3  :  // window
                    act->lightState = zone.getLightState();
                    break;
                case 4  :  // window
                case 5  :  // window
                case 6  :  // window
                case 7  :  // window
                    act->lightState = zone.getLightState();
                    break;
                case 8  :  // window
                case 9  :  // window
                case 10  :  // window
                case 11  :  // window
                    act->shadeState = zone.getBlindState();
                    break;
            }
            break;
        }
        learnNext = true;
    }
}
