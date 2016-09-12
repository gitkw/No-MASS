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
    void save();
    void setupSave();
    void setupPriority();
    void setGlobal(bool global);
    void setRecieved(const double r);
    void setRecievedCost(const double c);
    void setHourlyCost(const std::vector<double> & cost);
    void setHoulyPriority(const std::vector<double> & priority);
    bool isGlobal() const;
    double powerAt(const int timestep) const;
    double supplyAt(const int timestep) const;
    double supplyCostAt(const int timestep) const;
    double getPriorityAt(const int timestep) const;
    double powerBack() const;
    double supplyBack() const;


 protected:
    std::vector<double> power;
    std::vector<double> supply;
    std::vector<double> supplyCost;
    std::vector<double> recieved;
    std::vector<double> recievedCost;
    std::vector<double> hourlyCost;
    std::vector<double> priority;
    std::vector<double> hourlyPriority;
    bool global;
};

#endif  // APPLIANCE_H_
