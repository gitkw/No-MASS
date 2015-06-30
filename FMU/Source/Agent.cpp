/*
 * File:   Agent.cpp
 * Author: jake
 *
 * Created on September 15, 2013, 4:37 PM
 */

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "VariableStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"

#include "Agent.h"

Agent::Agent()
{
}

Agent::Agent(int newId) : id(newId)
{
    std::string idAsString = std::to_string(newId);
    VariableStore::addVariable("Agent_Activity_" + idAsString);
    agentStruct agent = SimulationConfig::agents.at(id);
    bedroom = agent.bedroom;
    office = agent.office;
    power = agent.power;

    if (SimulationConfig::info.windows){
        aaw.setup(agent.windowId);
    }
    if (SimulationConfig::info.shading){
        aas.setup(agent.shadeId);
    }
    aahg.setup(id);

    if (SimulationConfig::info.presencePage){
      model_presenceFromPage();
    }
    else{
      model_presenceFromActivities();
    }

}

void Agent::step(StateMachine *stateMachine)
{
    int stepCount = SimulationConfig::getStepCount();
    int newStateID = activities.at(stepCount);
    previousState = state;
    state = stateMachine->transistionTo(newStateID);
    Zone* zonePtr = state.getZonePtr();
    metabolicRate = state.getMetabolicRate();
    clo = state.getClo();
    activity = state.getActivity();

    if(presentAt(stepCount))
    {
        interactWithZone(*zonePtr);
    }

    if( stepCount > 0 && previousState.getId() != state.getId() && presentAt(stepCount - 1))
    {
        zonePtr = previousState.getZonePtr();
        interactWithZone(*zonePtr);
    }

    std::string name = "Agent_Activity_" + std::to_string(id);
    VariableStore::addValue(name.c_str(), activities.at(stepCount));



}

void Agent::interactWithZone(const Zone &zone)
{

    int theCase = SimulationConfig::info.caseOrder;
    switch(theCase)
    {
        case 0  :
        {
            random(zone);
            break; //optional
        }
        case 1  :
        {
            rLearn(zone);


            break; //optional
        }
    }

}

void Agent::random(const Zone &zone)
{

    // window ligth shade heat
    double i = Utility::randomDouble(0,1);
    double d = Utility::randomDouble(0,1);
    double s = Utility::randomDouble(0,1);
    double a = Utility::randomDouble(0,1);

    if(!SimulationConfig::info.lights){
        d= -1;
    }
    if(!SimulationConfig::info.windows){
        i = -1;
    }
    if(!SimulationConfig::info.shading){
        s = -1;
    }

    actions(zone, i, d, s, a);

}

void Agent::rLearn(const Zone &zone)
{
    double reward = -1;
    if( pmv == 0)
    {
        reward = 1;
    }
    // get new state
    // update q table that is learn
    rl.updateQ(pmv+3, action, reward, previous_pmv+3);
    // get action for current state using greedy
    action = rl.greedySelection(pmv + 3);
    // do action
    switch(action)
    {
    case 0  :
        // window ligth shade heat
        actions(zone, 9.0, 1.0, 0.8, 0.7);
        break; //optional
    case 1  :
        // window ligth shade heat
        actions(zone, 0.8, 1.0, 0.9, 0.7);
        break; //optional
    case 2  :
        // window ligth shade heat
        actions(zone, 0.8, 0.9, 1.0, 0.7);
        break; //optional
    case 3  :
        // window ligth shade heat
        actions(zone, 0.9, 0.8, 1.0, 0.7);
        break; //optional
    case 4  :
        // window ligth shade heat
        actions(zone, 1.0, 0.8, 0.9, 0.7);
        break; //optional
    case 5  :
        // window ligth shade heat
        actions(zone, 1.0, 0.9, 0.8, 0.7);
        break; //optional
    }

    previous_pmv = pmv;

}


void Agent::actions(const Zone &zone, double i, double d, double s, double a){

    interationStruct interaction;
    while(i+d+s+a > -4)
    {
        if(i > d && i > s && i > a)
        {
            interaction.windowState = calculateWindowInteractionsOnZone(zone);
            i = -1;
        }
        else if (d > s && d > i && d > a)
        {
            interaction.lightState = calculateLightInteractionsOnZone(zone);
            d = -1;
        }
        else if (s > i && s > d && s > a)
        {
            interaction.shadeState = calculateExternalShadeInteractionsOnZone(zone);
            s = -1;
        }
        else if (a > i && a > d && a > s)
        {
            interaction.heatgains =calculateMetabolicHeatGainsOnZone(zone);
            a = -1;
        }
    }
    zoneToInteraction[zone.getName()] = interaction;
}

void Agent::model_activity()
{
    activities = Model_Activity::getAgentActivities(id);
}

void Agent::model_presenceFromActivities()
{
    model_activity();
    presence = Model_Presence::calculatePresenceFromActivities(activities);
    model_pastAndFutureDurations();
}

void Agent::model_presenceFromPage()
{
    presence = Model_Presence::calculatePresenceFromPage();
    for (unsigned int i = 0; i < presence.size(); ++i)
    {
        if (presence.at(i))
        {
            activities.push_back(3);
        }
        else
        {
            activities.push_back(9);
        }
    }
    model_pastAndFutureDurations();
}

void Agent::model_pastAndFutureDurations()
{
    int lengthOfTimeStepSeconds = (60 * (60 / SimulationConfig::info.timeStepsPerHour));
    currentDurationOfPresenceState.push_back(0);

    for (unsigned int i = 1; i < presence.size(); ++i)
    {
        // std::cout << occupantFractions.at(i) << std::endl;
        bool currentPresence = presence.at(i) > 0;
        bool previousPresence = presence.at(i - 1) > 0;
        if (currentPresence == previousPresence)
        {
            currentDurationOfPresenceState.push_back(currentDurationOfPresenceState[i - 1] + lengthOfTimeStepSeconds);
        }
        else
        {
            currentDurationOfPresenceState.push_back(0);
        }
    }
    presenceForFutureSteps.assign(presence.size(), std::numeric_limits<int>::quiet_NaN());
    for (unsigned int i = 1; i+1 < presence.size(); ++i)
    {
        bool currentPresence = presence.at(i-1) > 0;
        bool futurePresence = presence.at(i) > 0;
        if (currentPresence && !futurePresence)
        {
            unsigned int j = i + 1;
            while (presence.at(j) == 0 && j < presence.size())
            {
                j++;
                if (j >= presence.size())
                {
                    break;
                }
            }
            presenceForFutureSteps[i] = currentDurationOfPresenceState[j - 1];
        }
    }
}

bool Agent::presentAt(int step) const
{
    return presence.at(step);
}

double Agent::getCurrentRadientGains() const
{
    return currentRadientGains;
}

double Agent::getPower() const
{
    return power;
}

bool Agent::getDesiredLightState(const Zone &zone) const
{
    return zoneToInteraction.at(zone.getName()).lightState;
}

bool Agent::getDesiredWindowState(const Zone &zone) const
{
    return zoneToInteraction.at(zone.getName()).windowState;
}

bool Agent::getDesiredShadeState(const Zone &zone) const
{
    return zoneToInteraction.at(zone.getName()).shadeState;
}

bool Agent::currentlyInZone(const Zone &zone) const
{
    return zone.getName() == state.getZonePtr()->getName();
}

bool Agent::previouslyInZone(const Zone &zone) const
{
    bool inZone = false;
    if (SimulationConfig::getStepCount() > 0)
    {
        inZone = zone.getName() == previousState.getZonePtr()->getName();
    }
    return inZone;
}

bool Agent::InteractionOnZone(const Zone &zone) const
{
    return currentlyInZone(zone) || previouslyInZone(zone);
}

int Agent::presentForFutureSteps() const
{
    return presenceForFutureSteps.at(SimulationConfig::getStepCount());
}

bool Agent::isInZone(std::string zoneName) const
{
    return (state.getZonePtr()->getName() == zoneName);
}

std::string Agent::getLocationType(int step, StateMachine *sm)
{
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    // std::cout << "Location is: " << s.getType() <<std::endl;
    return s.getActivity();
}

std::string Agent::getLocationName(int step, StateMachine *sm)
{
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    return updateLocation(s);
}

std::string Agent::updateLocation(const State& s) const
{
    std::string tempLocation = s.getLocation();
    if (s.getActivity() == "Sleeping")
    {
        tempLocation = bedroom;
    }
    else if(s.getActivity() == "IT" && SimulationConfig::info.presencePage)
    {
        if(office != "")
        {
            tempLocation = office;
        }
    }
    return tempLocation;
}

bool Agent::calculateLightInteractionsOnZone(const Zone &zone)
{
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);
    aal.step(zone, inZone, preZone, activities);
    return aal.getResult();
}

bool Agent::calculateWindowInteractionsOnZone(const Zone &zone)
{
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);
    aaw.step(zone, inZone, preZone, activities);
    return aaw.getResult();
}

double Agent::calculateExternalShadeInteractionsOnZone(const Zone &zone)
{
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);
    aas.step(zone, inZone, preZone, activities);
    return aas.getResult();
}

double Agent::calculateMetabolicHeatGainsOnZone(const Zone &zone)
{
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);
    aahg.step(zone, inZone, preZone, activities);
    pmv = aahg.getPMV();
    return aahg.getResult();
}

void Agent::postprocess()
{
    //rl.printQ();
}
