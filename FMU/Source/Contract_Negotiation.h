// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_NEGOTIATION_H_
#define CONTRACT_NEGOTIATION_H_

#include <unordered_map>
#include <vector>
#include "Contract.h"
#include "Contract_Priority_Node.h"
#include "Contract_Supply_Node.h"

class Contract_Negotiation {
 public:
    Contract_Negotiation();
    void submit(const Contract & c);
    void process();
    const Contract getContract(const int buildingID, const int id) const;
    double getRecievedPowerForContract(const int buildingID, const int id);
    double getCostOfPowerForContract(const int buildingID, const int id);
    void clear();
    double getDifference() const;

 private:
    void processContracts();
    bool sameContract(const ContractPtr c1, const ContractPtr c2) const;
    std::vector<int> suppliedIds;
    double difference;

    std::unordered_map<int, std::unordered_map<int, ContractPtr>> contracts;
    std::vector<ContractPtr> contractsSupplied;
    Contract_Supply_Node nodeSupply;
    Contract_Priority_Node nodePriority;
};

#endif  // CONTRACT_NEGOTIATION_H_
