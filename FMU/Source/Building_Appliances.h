// Copyright 2016 Jacob Chapman

#ifndef BUILDING_APPLIANCES_H_
#define BUILDING_APPLIANCES_H_

#include <vector>
#include <string>
#include "Appliance_Large.h"
#include "Appliance_Large_Learning.h"
#include "Appliance_Small.h"
#include "Appliance_FMI.h"
#include "Appliance_Battery.h"
#include "Appliance_Battery_GridCost_Reward.h"
#include "Appliance_Generic_CSV.h"
#include "Appliance.h"
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
    void stepLocalNegotiation();
    void stepGlobalNegotiation(
                        const Contract_Negotiation & building_negotiation);
    void postprocess();
    void preprocess();
    void postTimeStep();
    void addCurrentStates(const int stateid);

    double getTotalPower() const;
    void addContactsTo(Contract_Negotiation * building_negotiation);

 private:
    double PowerRequested;
    double PowerGenerated;
    double totalPower;
    int buildingID;

    std::vector<Appliance_Large> large;
    std::vector<Appliance_Large_Learning> largeLearning;
    std::vector<Appliance_Small> small;
    std::vector<Appliance_FMI> fmi;
    std::vector<Appliance_Battery> batteries;
    std::vector<Appliance_Battery_GridCost_Reward> batteriesGrid;
    std::vector<Appliance_Generic_CSV> csv;
    std::vector<int> currentStates;
    std::string buildingString;

    Contract_Negotiation app_negotiation;

    std::vector<Contract> globalContracts;

    double sum_large;
    double sum_cost;
    double sum_fmi;
    double sum_small;
    double sum_global_cost;
    void stepLocalSmall();
    void stepLocalLarge();
    void stepLocalLargeLearning();
    void stepLocalCSV();
    void stepLocalFMI();
    void stepLocalBatteries();
    void localNegotiationSmall();
    void localNegotiationLarge();
    void localNegotiationLargeLearning();
    void localNegotiationCSV();
    void localNegotiationFMI();
    void localNegotiationBatteries();
    void globalNegotiationLarge(
                          const Contract_Negotiation & building_negotiation);
    void globalNegotiationLargeLearning(
                          const Contract_Negotiation & building_negotiation);
    void globalNegotiationFMI(
                          const Contract_Negotiation & building_negotiation);
    void globalNegotiationBatteries(
                          const Contract_Negotiation & building_negotiation);
    void globalNegotiationSmall(
                          const Contract_Negotiation & building_negotiation);
    void globalNegotiationCSV(
                          const Contract_Negotiation & building_negotiation);
    bool sendContractLocal(const Appliance & a);
    bool sendContractGlobal(const Contract & c);
};

#endif  // BUILDING_APPLIANCES_H_
