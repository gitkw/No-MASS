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
    double supplyCostAt(const int timestep) const;
    double powerBack() const;
    double supplyBack() const;
    double getPriority() const;
    void setPriority(double priority);
    void setRecieved(const double r);
    void setHourlyCost(const std::vector<double> cost);
    void setRecievedCost(const double c);
    void save();
    void saveSetup();
    bool isGlobal() const;
    void setGlobal(bool global);

 protected:
    std::vector<double> power;
    std::vector<double> supply;
    std::vector<double> supplyCost;
    std::vector<double> recieved;
    std::vector<double> recievedCost;
    std::vector<double> hourlyCost;
    bool global;
    double priority;
};

#endif  // APPLIANCE_H_
