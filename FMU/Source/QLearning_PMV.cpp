#include "QLearning_PMV.h"

QLearning_PMV::QLearning_PMV()
{
    states = 7;
    actions = 3;
}



void QLearning_PMV::learn(const Zone &zone, ActionValues *act )
{
    bool doRLearn = false;
    if(zone.getWindowState() != act->windowState && zone.getLightState() != act->lightState){
        //std::cout << "window light" << std::endl;
        doRLearn = true;
    }else if( zone.getLightState() != act->lightState && zone.getBlindState() != act->shadeState){
        //std::cout << "light shade" << std::endl;
        doRLearn = true;
    }else if(zone.getBlindState() != act->shadeState && zone.getWindowState() != act->windowState){
        //std::cout << "shade window" << std::endl;
        doRLearn = true;
    }

    if(learnNext){

        double reward = -1;
        if( pmv == 0)
        {
            reward = 1;
        }
        // get new state
        // update q table that is learn
        ql->updateQ(pmv+3, action, reward, previous_pmv+3);

        learnNext = false;
    }



    if(doRLearn){
        // get action for current state using greedy
        action = ql->greedySelection(pmv + 3);
        switch(action)
        {
        case 0  ://window
            act->lightState = zone.getLightState();
            act->shadeState = zone.getBlindState();

            break;
        case 1  ://shade
            act->lightState = zone.getLightState();
            act->windowState = zone.getWindowState();

            break;
        case 2  ://light
            act->shadeState = zone.getBlindState();
            act->windowState = zone.getWindowState();

            break;
        }
        learnNext = true;
    }

}
