// Copyright 2016 Jacob Chapman

#include <vector>
#include <utility>
#include <iostream>
#include "LVN_Negotiation.h"


LVN_Negotiation::LVN_Negotiation() {}

void LVN_Negotiation::submit(contract c) {
  bool inserted = false;
  std::vector<contract>::iterator it;
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
}

void LVN_Negotiation::process() {
  std::vector<double> power;
  std::vector<double> cost;

  for (contract & c : contracts) {
    c.recieved = 0;
    c.recievedCost = 0;
    if (c.supplied > 0) {
      std::pair<double, double> p;
      power.push_back(c.supplied);
      cost.push_back(c.suppliedCost);
    }
  }

  for (contract & c : contracts) {
      double totalcost = 0;
      double totalrecieved = 0;
      double fractionalPower = c.requested;
      for (unsigned int i = 0; i < cost.size(); i++) {
          if (power[i] - fractionalPower > 0) {
              totalcost += cost[i] * fractionalPower;
              totalrecieved += fractionalPower;
              power[i] = power[i] - fractionalPower;
              break;
          } else {
              totalcost += cost[i] * power[i];
              totalrecieved += power[i];
              fractionalPower -= power[i];
              power[i] = 0.0;
          }
      }
      c.recievedCost = totalcost;
      c.recieved = totalrecieved;
  }
}

contract LVN_Negotiation::getContract(int id) {
  std::vector<contract>::iterator it;
  for (it = contracts.begin(); it < contracts.end(); it++) {
    if (it->id == id) {
      break;
    }
  }
  return *it;
}

double LVN_Negotiation::getRecievedPowerForContract(const int id) {
  return getContract(id).recieved;
}

double LVN_Negotiation::getCostOfPowerForContract(const int id) {
  return getContract(id).recievedCost;
}

void LVN_Negotiation::clear() {
  contracts.clear();
}
