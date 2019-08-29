// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_BATTERY_H_
#define APPLIANCE_BATTERY_H_

#include <vector>
#include "QLearning.hpp"
#include "Appliance.hpp"

/**
 * @brief Battery Appliance class
 * @details The Battery appliance agent
 */

class Appliance_Battery : public Appliance {
 public:
  Appliance_Battery();
  virtual ~Appliance_Battery() = default;

  void clear();
  void setup(ConfigStructAppliance a);
  void setupModel();
  void step();
  void stepNeighbourhood();
  void postprocess();
  void setPowerShortage(double power);
  void AddCost(double cost);
  void setBatteryNeighbourhoodDischarge(bool batteryNeighbourhoodDischarge);
  void setBatteryNeighbourhoodCharge(bool batteryNeighbourhoodCharge);
  virtual double rewardFunction(double mostShortage, double binShortage) const;
  double getStateOfCharge() const;

 protected:
  bool action;
  void doAction();
  void get_new_SOC_charge(double P_request);
  double get_new_SOC_discharge(double P_request);
  double energy_calc() const;
  double get_charge_delta() const;
  double calculateDeltaE(double P_request) const;
  void calculateStateOfCharge(double energy);
  void calculateSupply();
  void saveNeighbourhoodCalculate();
  void saveGlobalCalculate();
  double powerAvaliable(const double energy) const;
  int datastoreIDstateOfCharge;
  double chargeRate = 1000;
  double dischargeRate = 1000;
  double powerShortage;
  QLearning qLearning;
  int previousHourOfDay;
  double sumSupply;
  double sumShort;


  double mostShortage;
  double cost;
  double capacity = 2000;
  double efficiency = 0.98;
  double stateOfCharge;
  double BatteryDeltaT;

  bool batteryNeighbourhoodDischarge;
  bool batteryNeighbourhoodCharge;


};

#endif  // APPLIANCE_BATTERY_H_
