// Copyright 2016 Jacob Chapman

#ifndef BUILDING_APPLIANCES_H_
#define BUILDING_APPLIANCES_H_

#include <vector>
#include <string>
#include "Appliance_Battery_GridCost_Reward.h"
#include "Appliance_Battery.h"
#include "Appliance_Small.h"
#include "Appliance_Generic_CSV.h"
#include "Appliance_Large_Learning.h"
#include "Appliance_Large_Learning_CSV.h"
#include "Appliance_Large.h"
#include "Appliance_Large_CSV.h"
#include "Appliance_FMI.h"
#include "Appliance_Group.h"
#include "Appliance_Group_Battery.h"
#include "Contract_Negotiation.h"
#include "SimulationConfig.h"


/**
 * @brief Manages the different building appliances agents
 * @details Each building has a set of applaince this class manages them
 */
class Building_Appliances {
 public:
    Building_Appliances();
    void setup(const buildingStruct & b);
    void stepLocal();
    void stepAppliancesUseBatteries(
                        Contract_Negotiation * building_negotiation);
    void stepLocalNegotiation();
    void stepGlobalNegotiation(
                        const Contract_Negotiation & building_negotiation);

    void stepNeighbourhoodNegotiation(
                        const Contract_Negotiation & building_negotiation);

    void postprocess();
    void preprocess();
    void postTimeStep();
    void addCurrentStates(const int stateid);
    void addContactsTo(Contract_Negotiation * building_negotiation,
                        const bool battery);
    double getTotalPower() const;

 private:
    double totalPower;
    int buildingID;

    std::vector<int> currentStates;
    std::string buildingString;
    Contract_Negotiation app_negotiation;

    Appliance_Group<Appliance_Large> large;
    Appliance_Group<Appliance_Large_CSV> largeCSV;
    Appliance_Group<Appliance_Large_Learning> largeLearning;
    Appliance_Group<Appliance_Large_Learning_CSV> largeLearningCSV;
    Appliance_Group<Appliance_Small> small;
    Appliance_Group<Appliance_FMI> fmi;
    Appliance_Group<Appliance_Generic_CSV> csv;
    Appliance_Group_Battery<Appliance_Battery> batteries;
    Appliance_Group_Battery<Appliance_Battery_GridCost_Reward> batteriesGrid;

    int datastoreIDNonShiftSupplied;
    int datastoreIDNonShiftSuppliedCost;
    int datastoreIDNonShiftReceived;
    int datastoreIDNonShiftRequested;
    int datastoreIDNonShiftCost;
};

#endif  // BUILDING_APPLIANCES_H_
