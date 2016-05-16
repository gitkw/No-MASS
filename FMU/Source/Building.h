// Copyright 2015 Jacob Chapman

#ifndef BUILDING_H
#define	BUILDING_H
#include <vector>
#include <memory>
#include "SimulationConfig.h"
#include "Agent.h"
#include "StateMachine.h"
#include "Building_Zone.h"

class Building{
public:
    Building();

    void setup(const buildingStruct &b);
    void step();
    void postprocess();
    bool hasZone(const std::string& zoneName) const;

private:
    void setAgentGainsForZone(Building_Zone *zone);
    void setAgentWindowDecisionForZone(Building_Zone *zone);
    void setAgentLightDecisionForZone(Building_Zone *zone);
    void setAgentBlindDecisionForZone(Building_Zone *zone);
    void setAgentHeatDecisionsForZone(Building_Zone *zone);
    void setAgentCountForZone(Building_Zone *zone);
    void initialiseStates();
    void setZones(const std::vector<Building_Zone> & zones);
    void buildingInteractions();
    void matchStateToZone(State &s);
    std::vector<float> presenceProfile;
    std::vector<Agent> population;
    StateMachine stateMachine;
    std::vector<Building_Zone> zones;
    std::string name;
    int x = 0;
};

#endif	/* BUILDING_H */
