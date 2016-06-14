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
    void postTimeStep();

    static double dailyMeanTemperature;

private:
    void setAgentGainsForZone(std::shared_ptr<Building_Zone> zone);
    void setAgentWindowDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setAgentLightDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setAgentBlindDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setAgentHeatDecisionsForZone(std::shared_ptr<Building_Zone> zone);
    void setAgentCountForZone(std::shared_ptr<Building_Zone> zone);
    void setAppGainsForZone(std::shared_ptr<Building_Zone> zone);
    void initialiseStates();
    void setZones(const std::vector<std::shared_ptr<Building_Zone>> & zones);
    void buildingInteractions();
    void matchStateToZone(State &s);
    std::vector<float> presenceProfile;
    std::vector<Agent> population;
    StateMachine stateMachine;
    std::vector<std::shared_ptr<Building_Zone>> zones;
    std::string name;

    std::deque<double> outDoorTemperatures;

};

#endif	/* BUILDING_H */
