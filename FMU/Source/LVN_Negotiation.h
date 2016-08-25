// Copyright 2016 Jacob Chapman

#ifndef LVN_NEGOTIATION_H_
#define LVN_NEGOTIATION_H_

#include <vector>
#include <map>

struct contract {
    int id;
    int buildingID;
    double requested = 0.0;
    double recieved = 0.0;
    double recievedCost = 0.0;
    double supplied = 0.0;
    double suppliedCost = 0.0;
    double suppliedLeft = 0.0;
    double priority = 0.0;
};

class LVN_Negotiation {
 public:
    LVN_Negotiation();
    void submit(contract c);
    void process();
    contract getContract(const int buildingID, const int id) const;
    double getRecievedPowerForContract(const int buildingID, const int id);
    double getCostOfPowerForContract(const int buildingID, const int id);
    void clear();
    double getDifference() const;

 private:
    void processContracts();
    int bestSupply();
    std::vector<contract> contracts;
    double difference;
};

#endif  // LVN_NEGOTIATION_H_
