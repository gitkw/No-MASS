// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_H_
#define APPLIANCE_H_

#include <vector>
#include "Agent.h"


struct ApplianceParameters {
  double power = 0.0;
  double supply = 0.0;
  double supplyCost = 0.0;
  double received = 0.0;
  double receivedCost = 0.0;
};

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
    void setReceived(const double r);
    void setReceivedCost(const double c);
    void setHourlyCost(const std::vector<double> & cost);
    void setHoulyPriority(const std::vector<double> & priority);
    bool isGlobal() const;
    bool isLocal() const;
    double getSupply() const;
    double getSupplyCost() const;
    double getReceived() const;
    double getReceivedCost() const;
    double getPriority() const;
    double getPower() const;
    int calculateHourOfDay() const;

 protected:

    void setPower(const double power);
    void setSupply(const double supply);
    void setSupplyCost(const double supplyCost);

    std::vector<double> hourlyCost;
    std::vector<double> hourlyPriority;
    bool global;
    bool local;

    int datastoreIDSupplied;
    int datastoreIDSuppliedCost;
    int datastoreIDReceived;
    int datastoreIDRequested;
    int datastoreIDCost;

private:
    ApplianceParameters parameters;
};

#endif  // APPLIANCE_H_
