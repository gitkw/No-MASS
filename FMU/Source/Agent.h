/*
 * File:   Agent.h
 * Author: jake
 *
 * Created on September 15, 2013, 4:37 PM
 */

#ifndef AGENT_H
#define	AGENT_H
#include <unordered_map>
#include <string>
#include <memory>
#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"

#include "Model_Lights.h"
#include "Model_ExternalShading.h"
#include "RLearning.h"
#include "Agent_Action_Window.h"
#include "Agent_Action_Lights.h"
#include "Agent_Action_Shades.h"
#include "Agent_Action_Heat_Gains.h"
/**
 * @brief The Agent
 * @details Contains all information about the occupants and there associated interactions
 */

struct interationStruct {
    bool lightState;
    bool windowState;
    double shadeState;
    double ppd;
    double pmv;
    double heatgains;
};

class Agent {
public:
    Agent();
    Agent(int newId);

    void step(StateMachine *sm);
    void zoneInteractions();
    void postprocess();
    void model_presenceFromActivities();
    void model_presenceFromPage();
    bool presentAt(int step) const;
    bool isInZone(std::string zoneName) const;
    bool InteractionOnZone(const Zone &zone) const;
    int presentForFutureSteps() const;
    double getCurrentRadientGains() const;
    double getPower() const;

    void interactWithZone(const Zone &zone);
    bool getDesiredLightState(const Zone &zone) const;
    bool getDesiredWindowState(const Zone &zone) const;
    bool getDesiredShadeState(const Zone &zone) const;
    std::string getLocationType(int step,StateMachine *sm);
    std::string getLocationName(int step,StateMachine *sm);

private:
    int id;
    int pmv;
    int previous_pmv;
    int action;

    double power;
    double currentRadientGains;
    std::vector<double> activities;
    std::vector<int> presence;
    std::vector<int> presenceForFutureSteps;
    std::vector<int> currentDurationOfPresenceState;

    double metabolicRate; /** Metabolic Rate of the occupant */
    double clo; /** clothing value of the occupant */

    std::string bedroom; /** Which bedroom the occupant sleeps in */
    std::string office; /** WhichoOffice the occupant works in */
    std::string activity; /** The activities performed in the location */
    State state; /** Occupants current state */
    State previousState; /** Occupants previous state */
/*
    Agent_Action_Window aaw;
    Agent_Action_Lights aal;
    Agent_Action_Shades aas;
    Agent_Action_Heat_Gains aahg;
*/
    std::vector<std::unique_ptr<Agent_Action>> available_actions;

    RLearning rl;
    bool learn = false;

    std::unordered_map<std::string, interationStruct> zoneToInteraction;
    void model_pastAndFutureDurations();
    void model_activity();

    void rLearn(const Zone &zone, interationStruct *is);

    std::string updateLocation(const State& state) const;
    bool currentlyInZone(const Zone &zone) const;
    bool previouslyInZone(const Zone &zone) const;
    bool calculateLightInteractionsOnZone(const Zone &zone);
    bool calculateWindowInteractionsOnZone(const Zone &zone);
    double calculateExternalShadeInteractionsOnZone(const Zone &zone);
    double calculateMetabolicHeatGainsOnZone(const Zone &zone);
    double getPreviousDurationOfAbsenceState() const;
    double getCurrentDurationOfPresenceState() const;
    double getFutureDurationOfPresenceState() const;


};

#endif	/* AGENT_H */
