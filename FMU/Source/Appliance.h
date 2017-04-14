// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_H_
#define APPLIANCE_H_

#include <vector>
#include "SimulationConfig.h"
#include "Agent.h"



struct ApplianceParameters {
  double power = 0.0;
  double supply = 0.0;
  double supplyCost = 0.0;
  double suppliedLeft = 0.0;
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
    virtual void setup(applianceStruct a) = 0;
    void save();
    void clear();
    void setupSave();
    void setGlobal(bool global);
    void setLocal(bool local);
    void setReceived(const double r);
    void setReceivedCost(const double c);
    void setSupplyLeft(const double supplyLeft);
    void setPower(const double power);
    void setSupply(const double supply);
    void setSupplyCost(const double supplyCost);
    void setHourlyCost(const std::vector<double> & cost);
    void setHoulyPriority(const std::vector<double> & priority);
    bool isGlobal() const;
    bool isLocal() const;
    double getSupply() const;
    double getSupplyLeft() const;
    double getSupplyCost() const;
    double getReceived() const;
    double getReceivedCost() const;
    double getPriority() const;
    double getPower() const;
    int calculateHourOfDay() const;

    void saveLocal();
    void saveNeighbourhood();
    void saveGlobal();

    double getLocalSupply() const;
    double getLocalSupplyLeft() const;
    double getLocalReceived() const;
    double getLocalReceivedCost() const;
    double getLocalPower() const;

    double getNeighbourhoodSupply() const;
    double getNeighbourhoodSupplyLeft() const;
    double getNeighbourhoodReceived() const;
    double getNeighbourhoodReceivedCost() const;
    double getNeighbourhoodPower() const;

    double getGridSupply() const;
    double getGridSupplyLeft() const;
    double getGridReceived() const;
    double getGridReceivedCost() const;
    double getGridPower() const;


    void setActivities(const std::vector<int> Activities);
    bool hasActivities(const std::vector<int> Activities);

 protected:
    virtual void beforeClear();
    virtual void saveNeighbourhoodCalculate();
    virtual void saveGlobalCalculate();

    std::vector<double> hourlyCost;
    std::vector<double> hourlyPriority;
    std::vector<int> Activities;

    bool global;
    bool local;
    bool match = true;

    int datastoreIDSupplied;
    int datastoreIDSuppliedCost;
    int datastoreIDReceived;
    int datastoreIDRequested;
    int datastoreIDCost;

    int datastoreLocalIDSupplied;
    int datastoreLocalIDSuppliedCost;
    int datastoreLocalIDReceived;
    int datastoreLocalIDRequested;
    int datastoreLocalIDCost;
    int datastoreNeighbourhoodIDSupplied;
    int datastoreNeighbourhoodIDSuppliedCost;
    int datastoreNeighbourhoodIDReceived;
    int datastoreNeighbourhoodIDRequested;
    int datastoreNeighbourhoodIDCost;
    int datastoreGridIDSupplied;
    int datastoreGridIDSuppliedCost;
    int datastoreGridIDReceived;
    int datastoreGridIDRequested;
    int datastoreGridIDCost;

    ApplianceParameters parameters;
    ApplianceParameters parametersLocal;
    ApplianceParameters parametersNeighbourhood;
    ApplianceParameters parametersGrid;
 private:
};

#endif  // APPLIANCE_H_
