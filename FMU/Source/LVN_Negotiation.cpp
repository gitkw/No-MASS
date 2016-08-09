// Copyright 2016 Jacob Chapman

#include <vector>
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
  totalPower = 0;
  demandedPower = 0;
  for (contract & c : contracts) {
    totalPower += c.supplied;
    demandedPower += c.requested;
    c.recieved = 0;
  }
  for (contract & c : contracts) {
    if (totalPower >= c.requested) {
      totalPower -= c.requested;
      c.recieved = c.requested;
    }
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

void LVN_Negotiation::clear() {
  contracts.clear();
}
