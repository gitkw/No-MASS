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
    void clear();
    void setupSave();
    void setGlobal(bool global);
    void setLocal(bool local);
    void setRecieved(const double r);
    void setRecievedCost(const double c);
    void setHourlyCost(const std::vector<double> & cost);
    void setHoulyPriority(const std::vector<double> & priority);
    bool isGlobal() const;
    bool isLocal() const;
    double getSupply() const;
    double getSupplyCost() const;
    double getPriority() const;
    double getPower() const;
    int calculateHourOfDay() const;



 protected:
    double power = 0.0;
    double supply = 0.0;
    double supplyCost = 0.0;
    double recieved = 0.0;
    double recievedCost = 0.0;
    std::vector<double> hourlyCost;
    std::vector<double> hourlyPriority;
    bool global;
    bool local;

    int datastoreIDSupplied;
    int datastoreIDSuppliedCost;
    int datastoreIDRecieved;
    int datastoreIDRequested;
    int datastoreIDCost;
};

#endif  // APPLIANCE_H_
