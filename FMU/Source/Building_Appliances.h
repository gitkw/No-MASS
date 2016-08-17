// Copyright 2016 Jacob Chapman

#ifndef BUILDING_APPLIANCES_H_
#define BUILDING_APPLIANCES_H_

#include <vector>
#include <string>
#include "Appliance_Large.h"
#include "Appliance_Large_Learning.h"
#include "Appliance_Grid.h"
#include "Appliance_Small.h"
#include "Appliance_FMI.h"
#include "Appliance_PV.h"
#include "LVN_Negotiation.h"

/**
 * @brief Manages the different building appliances agents
 * @details Each building has a set of applaince this class manages them
 */
class Building_Appliances {
 public:
    Building_Appliances();
    void setup();
    void step();
    void postprocess();
    void preprocess();
    void postTimeStep();
    void stepLarge();
    void setBuildingID(const int id);
    void addCurrentStates(const int stateid);
    void sendContract(int id, double priority, double request,
                              double supply, double cost);
    double getTotalPower() const;

 private:
    int buildingID;
    double PowerRequested;
    double PowerGenerated;
    double totalPower;

    std::vector<Appliance_Large> large;
    std::vector<Appliance_Large_Learning> largeLearning;
    std::vector<Appliance_Grid> grid;
    std::vector<Appliance_Small> small;
    std::vector<Appliance_FMI> fmi;
    std::vector<Appliance_PV> pv;
    std::vector<int> currentStates;
    std::string buildingString;

    LVN_Negotiation app_negotiation;

    void stepSmall();
    void stepLargeLearning();
    void stepPV();
    void stepFMI();
    void stepGrid();
    void addAppToDataStrore(const int id);
    void addAppVariableToDataStrore(const int id, const int requested,
                                    const int supplied);
    void addAppRecievedToDataStrore(const int id, const int recieved,
                                    const int cost);
};

#endif  // BUILDING_APPLIANCES_H_
