// Copyright 2016 Jacob Chapman

#ifndef LVN_NEGOTIATION_H_
#define LVN_NEGOTIATION_H_

#include <vector>

struct contract {
    int id;
    double requested;
    double recieved;
    double supplied;
    double priority;
};

class LVN_Negotiation {
 public:
    LVN_Negotiation();
    void submit(contract c);
    void process();
    contract getContract(const int id);
    double getRecievedPowerForContract(const int id);
    void clear();

 private:
    double totalPower;
    double demandedPower;
    std::vector<contract> contracts;
};

#endif  // LVN_NEGOTIATION_H_
