// Copyright 2016 Jacob Chapman

#ifndef OCCUPANT_H
#define	OCCUPANT_H
#include <unordered_map>
#include <string>
#include <memory>

#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"

#include "Agent.h"
#include "Occupant_Zone.h"

class Occupant : public Agent {
public:
    Occupant();
    void setup(int newId, const std::vector<std::shared_ptr<Building_Zone>> &zones);

    void step();
    void setState(State &state);
    void setBuildingID(const int buildingID);
    void postTimeStep();
    void zoneInteractions();
    void postprocess();
    bool currentlyInZone(const Building_Zone &zone) const;
    bool InteractionOnZone(const Building_Zone &zone) const;
    bool getDesiredLightState(const Building_Zone &zone) const;
    bool getDesiredWindowState(const Building_Zone &zone) const;
    bool isActionWindow(const Building_Zone &zone) const;
    bool isActionLights(const Building_Zone &zone) const;
    bool isActionShades(const Building_Zone &zone) const;
    bool isActionHeatGains(const Building_Zone &zone) const;
    bool isActionLearning(const Building_Zone &zone) const;
    bool isActionAppliance(const Building_Zone &zone) const;
    double getDesiredShadeState(const Building_Zone &zone) const;
    double getDesiredAppliance(const Building_Zone &zone) const;
    double getDesiredHeatState(const Building_Zone &zone) const;
    double getCurrentRadientGains(const Building_Zone &zone) const;
    double getPower() const;

    std::string getLocationType(int step, StateMachine *sm);
    std::string getLocationName(int step, StateMachine *sm);

    bool previouslyInZone(const Building_Zone &zone) const;
private:

    int id;
    int action;
    bool heatState;
    bool learn = false;
    double power;
    double currentRadientGains;
    double metabolicRate; /** Metabolic Rate of the occupant */
    double clo; /** clothing value of the occupant */

    std::vector<Occupant_Zone> agentZones;
    std::vector<double> activities;
    std::vector<int> availableActions;
    int buildingID;
    std::string buildingName; /** building agent belongs to */
    std::string bedroom; /** Which bedroom the occupant sleeps in */
    std::string office; /** Which Office the occupant works in */
    State state; /** Occupants current state */
    std::shared_ptr<Building_Zone> zonePtrPrevious;

    void model_presenceFromPage();
    void model_pastAndFutureDurations();
    void model_activity();
    void matchStateToZone(State &s, const std::vector<std::shared_ptr<Building_Zone>> &zones);

    void initialiseStates(const std::vector<std::shared_ptr<Building_Zone>> &zones);
    bool calculateLightInteractionsOnZone(const Building_Zone &zone);
    bool calculateWindowInteractionsOnZone(const Building_Zone &zone);
    double calculateExternalShadeInteractionsOnZone(const Building_Zone &zone);
    double calculateMetabolicHeatGainsOnZone(const Building_Zone &zone);
    double getPMV(const Building_Zone &zone) const;
    std::string updateLocation(const State& state) const;

    StateMachine stateMachine;
};

#endif	/* OCCUPANT_H */
