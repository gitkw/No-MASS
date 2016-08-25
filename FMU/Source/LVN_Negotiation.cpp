// Copyright 2016 Jacob Chapman

#include <vector>
#include <utility>
#include <limits>
#include <iostream>
#include "LVN_Negotiation.h"


LVN_Negotiation::LVN_Negotiation() {
    difference = 0;
}

void LVN_Negotiation::submit(contract c) {
  bool inserted = false;
  std::vector<contract>::iterator it;
  c.suppliedLeft = c.supplied;
  for (it = contracts.begin(); it < contracts.end(); it++) {
    if (c.priority > it->priority) {
      inserted = true;
      contracts.insert(it, c);
      break;
    }
  }
  if (!inserted) {
    contracts.push_back(c);
  }
  difference += c.supplied - c.requested;
}
double LVN_Negotiation::getDifference() const {
  return difference;
}

void LVN_Negotiation::process() {
    processContracts();
}

int LVN_Negotiation::bestSupply() {
  int best = -1;
  double cost = std::numeric_limits<double>::max();
  for (unsigned int i = 0; i < contracts.size(); i++) {
    if (contracts[i].suppliedLeft > 0 && contracts[i].suppliedCost < cost) {
      best = i;
      cost = contracts[i].suppliedCost;
    }
  }
  return best;
}

/**
 * @brief Assign power to the contracts
 * @details going slow? have you cleared the contracts?
 */
void LVN_Negotiation::processContracts() {
  for (contract & c : contracts) {
      double totalcost = 0;
      double totalrecieved = 0;
      double fractionalPower = c.requested;
      while (fractionalPower > 0) {
          int i = bestSupply();
          if (i < 0) {
            break;
          }
          if (contracts[i].suppliedLeft - fractionalPower > 0) {
              totalcost += contracts[i].suppliedCost * fractionalPower;
              totalrecieved += fractionalPower;
              contracts[i].suppliedLeft -= fractionalPower;
              break;
          } else {
              double p = contracts[i].suppliedLeft;
              totalcost += contracts[i].suppliedCost * p;
              totalrecieved += p;
              fractionalPower -= p;
              contracts[i].suppliedLeft = 0.0;
          }
      }
      c.recievedCost += totalcost;
      c.recieved += totalrecieved;
  }
}

contract LVN_Negotiation::getContract(const int buildingID, int id) const {
    std::vector<contract>::const_iterator it;
  for (it = contracts.cbegin(); it < contracts.cend(); it++) {
    if (it->id == id && it->buildingID == buildingID) {
      break;
    }
  }
  return *it;
}

double LVN_Negotiation::getRecievedPowerForContract(const int buildingID,
                                                      const int id) {
  return getContract(buildingID, id).recieved;
}

double LVN_Negotiation::getCostOfPowerForContract(const int buildingID,
                                                  const int id) {
  return getContract(buildingID, id).recievedCost;
}

void LVN_Negotiation::clear() {
  contracts.clear();
  difference = 0;
}
