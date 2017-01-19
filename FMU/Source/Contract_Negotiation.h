// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_NEGOTIATION_H_
#define CONTRACT_NEGOTIATION_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include "Contract.h"
#include "Contract_Node_Priority.h"
#include "Contract_Node_Supply.h"

class Contract_Negotiation {
 public:
    Contract_Negotiation();
    void submit(const Contract & c);
    void process();
    const Contract getContract(const int buildingID, const int id) const;
    double getReceivedPowerForContract(const int buildingID, const int id);
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
    Contract_Node_Supply nodeSupply;
    Contract_Node_Priority nodePriority;
};

#endif  // CONTRACT_NEGOTIATION_H_
