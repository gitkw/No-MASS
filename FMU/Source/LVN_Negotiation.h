// Copyright 2016 Jacob Chapman

#ifndef LVN_NEGOTIATION_H_
#define LVN_NEGOTIATION_H_

#include <vector>
#include <map>

struct contract {
    int id;
    double requested;
    double recieved;
    double recievedCost;
    double supplied;
    double suppliedCost;
    double priority;
};

class LVN_Negotiation {
 public:
    LVN_Negotiation();
    void submit(contract c);
    void process();
    contract getContract(const int id);
    double getRecievedPowerForContract(const int id);
    double getCostOfPowerForContract(const int id);
    void clear();

 private:
    std::vector<contract> contracts;
};

#endif  // LVN_NEGOTIATION_H_
