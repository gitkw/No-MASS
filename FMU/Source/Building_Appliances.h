// Copyright 2016 Jacob Chapman

#ifndef BUILDING_APPLIANCES_H_
#define BUILDING_APPLIANCES_H_

#include <vector>
#include <string>
#include "Appliance_Large.h"
#include "Appliance_Large_Learning.h"
#include "Appliance_Small.h"
#include "Appliance_FMI.h"
#include "Appliance_PV.h"
#include "Appliance.h"
#include "LVN_Negotiation.h"
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
    void stepGlobalNegotiation(const LVN_Negotiation & building_negotiation);
    void postprocess();
    void preprocess();
    void postTimeStep();
    void addCurrentStates(const int stateid);

    double getTotalPower() const;
    void addContactsTo(LVN_Negotiation * building_negotiation);

 private:
    double PowerRequested;
    double PowerGenerated;
    double totalPower;
    int buildingID;

    std::vector<Appliance_Large> large;
    std::vector<Appliance_Large_Learning> largeLearning;
    std::vector<Appliance_Small> small;
    std::vector<Appliance_FMI> fmi;
    std::vector<Appliance_PV> pv;
    std::vector<int> currentStates;
    std::string buildingString;

    LVN_Negotiation app_negotiation;

    std::vector<contract> globalContracts;

    double sum_large;
    double sum_cost;
    double sum_fmi;
    double sum_small;
    void stepLocalSmall();
    void stepLocalLarge();
    void stepLocalLargeLearning();
    void stepLocalPV();
    void stepLocalFMI();
    void localNegotiationSmall();
    void localNegotiationLarge();
    void localNegotiationLargeLearning();
    void localNegotiationPV();
    void localNegotiationFMI();
    void globalNegotiationLarge(const LVN_Negotiation & building_negotiation);
    void globalNegotiationLargeLearning(const LVN_Negotiation & building_negotiation);
    void globalNegotiationFMI(const LVN_Negotiation & building_negotiation);
    void globalNegotiationSmall(const LVN_Negotiation & building_negotiation);
    void globalNegotiationPV(const LVN_Negotiation & building_negotiation);
    void sendContractLocal(const Appliance & a);
    bool sendContractGlobal(const contract & c);
};

#endif  // BUILDING_APPLIANCES_H_
