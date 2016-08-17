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

 protected:
    std::vector<double> power;
    std::vector<double> supply;

    double cost;
    double priority;
};

#endif  // APPLIANCE_H_
