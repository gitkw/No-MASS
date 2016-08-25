// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_H_
#define APPLIANCE_H_

#include <vector>
#include "Agent.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */

class Appliance : public Agent {
 public:
    Appliance();
    double powerAt(const int timestep) const;
    double supplyAt(const int timestep) const;
    double powerBack() const;
    double supplyBack() const;
    double getPriority() const;
    void setPriority(double priority);
    double getCost() const;
    void setCost(double cost);
    void setRecieved(const double r);
    void setRecievedCost(const double c);
    void save();
    void saveSetup();
    bool isGlobal() const;
    void setGlobal(bool global);

 protected:
    std::vector<double> power;
    std::vector<double> supply;
    std::vector<double> recieved;
    std::vector<double> recievedCost;
    bool global;
    double cost;
    double priority;
};

#endif  // APPLIANCE_H_
