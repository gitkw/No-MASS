// Copyright 2016 Jacob Chapman

#include <vector>
#include <utility>
#include <limits>
#include <iostream>
#include "Contract_Negotiation.h"

Contract_Negotiation::Contract_Negotiation() {
    difference = 0;
}

void Contract_Negotiation::submit(const Contract & c) {
  contracts[c.buildingID][c.id] = std::make_shared<Contract>(c);
  ContractPtr contract = contracts.at(c.buildingID).at(c.id);
  contract->suppliedLeft = contract->supplied;
  if (contract->requested > 0) {
    nodePriority.insert(contract, contract->priority);
  }
  if (contract->supplied > 0) {
    nodeSupply.insert(contract, contract->suppliedCost);
  }
  difference += contract->supplied - contract->requested;
}

double Contract_Negotiation::getDifference() const {
  return difference;
}

void Contract_Negotiation::process() {
    ContractPtr cs = nodeSupply.popLeftEdge();
    if (cs) {
      while (cs) {
        contractsSupplied.push_back(cs);
        cs = nodeSupply.popLeftEdge();
      }
      processContracts();
    }
}


/**
 * @brief Assign power to the contracts
 * @details going slow? have you cleared the contracts?
 */
void Contract_Negotiation::processContracts() {
  ContractPtr c = nodePriority.findRightEdge();
  std::vector<ContractPtr>::iterator cs = contractsSupplied.begin();
  while (c && cs!= contractsSupplied.end()) {
      double fractionalPower = c->requested - c->recieved;
      while (fractionalPower > 0 && cs != contractsSupplied.end()) {
          if (sameContract((*cs), c)) {
            cs++;
            continue;
          }
          double supplied = (*cs)->suppliedLeft;
          double suppliedCost = (*cs)->suppliedCost;
          if (supplied - fractionalPower > 0) {
            supplied = fractionalPower;
            (*cs)->suppliedLeft -= supplied;
          } else {
            contractsSupplied.erase(cs);
          }
          c->recievedCost += suppliedCost * supplied;
          c->recieved += supplied;
          fractionalPower -= supplied;
          cs = contractsSupplied.begin();
      }
      c = nodePriority.findRightEdge();
  }
}

bool Contract_Negotiation::sameContract(const ContractPtr c1,
                                          const ContractPtr c2) const {
  return ((c1->buildingID == c2->buildingID) && (c1->id == c2->id));
}

const Contract Contract_Negotiation::getContract(
                                  const int buildingID, const int id) const {
  return *contracts.at(buildingID).at(id).get();
}

double Contract_Negotiation::getRecievedPowerForContract(const int buildingID,
                                                      const int id) {
  return getContract(buildingID, id).recieved;
}

double Contract_Negotiation::getCostOfPowerForContract(const int buildingID,
                                                  const int id) {
  return getContract(buildingID, id).recievedCost;
}

void Contract_Negotiation::clear() {
  contracts.clear();
  nodeSupply.clear();
  nodePriority.clear();
  contractsSupplied.clear();
  difference = 0;
}
