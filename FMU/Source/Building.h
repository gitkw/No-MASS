// Copyright 2015 Jacob Chapman

#ifndef BUILDING_H
#define	BUILDING_H
#include <vector>
#include <memory>
#include "SimulationConfig.h"
#include "Agent.h"
#include "StateMachine.h"
#include "Zone.h"

class Building{
public:
    Building();

    void setup(const buildingStruct &b);
    void step();
    void postprocess();

private:
    void setAgentGainsForZone(Zone *zone);
    void setAgentWindowDecisionForZone(Zone *zone);
    void setAgentLightDecisionForZone(Zone *zone);
    void setAgentBlindDecisionForZone(Zone *zone);
    void setAgentCountForZone(Zone *zone);
    void initialiseStates();
    void setZones(std::vector<Zone> zones);

    void matchStateToZone(State &s);
    std::vector<float> presenceProfile;
    std::vector<Agent> population;
    StateMachine stateMachine;
    std::vector<Zone> zones;
    std::string name;
};

#endif	/* BUILDING_H */
