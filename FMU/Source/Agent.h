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

#include "Agent_Zone.h"
#include "Agent_Action_Learning.h"
#include "Agent_Action_Heating.h"
#include "Agent_Action_Shades.h"
#include "Agent_Action_Heat_Gains.h"
/**
 * @brief The Agent
 * @details Contains all information about the occupants and there associated interactions
 */

struct desires {
    double shadeState;
    double ppd;
    double pmv;
    double heatgains;
    double heatingSetPoint;
};

class Agent {
public:
    Agent();
    Agent(int newId, const std::vector<Building_Zone> &zone);

    void step(StateMachine *sm);
    void setState(State &state);
    void zoneInteractions();
    void postprocess();
    void model_presenceFromActivities();
    void model_presenceFromPage();
    void interactWithZone(const Building_Zone &zone);

    bool currentlyInZone(const Building_Zone &zone) const;
    bool InteractionOnZone(const Building_Zone &zone) const;
    bool getDesiredLightState(const Building_Zone &zone) const;
    bool getDesiredWindowState(const Building_Zone &zone) const;
    double getDesiredShadeState(const Building_Zone &zone) const;
    double getDesiredHeatState(const Building_Zone &zone) const;
    double getCurrentRadientGains(const Building_Zone &zone) const;
    double getPower() const;

    std::string getLocationType(int step, StateMachine *sm);
    std::string getLocationName(int step, StateMachine *sm);

private:

    int id;
    int action;
    bool heatState;
    bool learn = false;
    double power;
    double currentRadientGains;
    double metabolicRate; /** Metabolic Rate of the occupant */
    double clo; /** clothing value of the occupant */

    std::vector<Agent_Zone> agentZones;
    std::vector<double> activities;
    std::vector<int> availableActions;
    std::unordered_map<std::string, desires> zoneToInteraction;

    std::string bedroom; /** Which bedroom the occupant sleeps in */
    std::string office; /** Which Office the occupant works in */

    Agent_Action_Heating aah;
    Agent_Action_Learning aalearn;

    Model_Presence presence;
    State state; /** Occupants current state */
    State previousState; /** Occupants previous state */

    void model_pastAndFutureDurations();
    void model_activity();
    void rLearn(const Building_Zone &zone, desires *interaction);
    bool previouslyInZone(const Building_Zone &zone) const;
    bool calculateLightInteractionsOnZone(const Building_Zone &zone);
    bool calculateWindowInteractionsOnZone(const Building_Zone &zone);
    double calculateExternalShadeInteractionsOnZone(const Building_Zone &zone);
    double calculateMetabolicHeatGainsOnZone(const Building_Zone &zone);
    double getPreviousDurationOfAbsenceState() const;
    double getCurrentDurationOfPresenceState() const;
    double getFutureDurationOfPresenceState() const;
    std::string updateLocation(const State& state) const;
    double getPMV(const Building_Zone &zone) const;

};

#endif	/* AGENT_H */
