// Copyright 2015 Jacob Chapman

#ifndef BUILDING_H
#define	BUILDING_H
#include <vector>
#include <deque>
#include <memory>
#include "SimulationConfig.h"
#include "Occupant.h"
#include "Building_Zone.h"
#include "Building_Appliances.h"

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
    void setOccupantGainsForZone(std::shared_ptr<Building_Zone> zone);
    void setOccupantWindowDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setOccupantLightDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setOccupantBlindDecisionForZone(std::shared_ptr<Building_Zone> zone);
    void setOccupantHeatDecisionsForZone(std::shared_ptr<Building_Zone> zone);
    void setOccupantCountForZone(std::shared_ptr<Building_Zone> zone);
    void setAppGainsForZone(std::shared_ptr<Building_Zone> zone);

    void setZones(const std::vector<std::shared_ptr<Building_Zone>> & zones);
    void buildingInteractions();
    void calculateDailyMeanTemperature();
    std::vector<float> presenceProfile;
    std::vector<Occupant> population;
    std::vector<std::shared_ptr<Building_Zone>> zones;
    std::string name;
    std::deque<double> outDoorTemperatures;

    Building_Appliances appliances;

    int id;


};

#endif	/* BUILDING_H */
