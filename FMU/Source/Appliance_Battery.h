// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_BATTERY_H_
#define APPLIANCE_BATTERY_H_

#include <vector>
#include "QLearning_Appliance.h"
#include "Appliance.h"

/**
 * @brief Battery Appliance class
 * @details The Battery appliance agent
 */

class Appliance_Battery : public Appliance {
 public:
  Appliance_Battery();

  void clear();
  void setup();
  void setupModel();
  void step();
  void postprocess();
  void setPowerShortage(double power);
  void setEpsilon(double epsilon);
  void setAlpha(double alpha);
  void setGamma(double gamma);
  void setUpdate(bool update);
  void AddCost(double cost);

 private:
  int datastoreIDstateOfCharge;
  double chargeRate = 1000;
  double dischargeRate = 1000;
  double powerShortage;
  QLearning_Appliance qLearning;
  double epsilon;   // probability of a random action selection
  double alpha;     // learning rate
  double gamma;     // discount factor (how soon do you care)
  bool update;
  int previousHourOfDay;
  double sumSupply;
  double sumShort;
  bool action;
  
  double mostShortage;
  double cost;
  double capacity = 2000;
  double efficiency = 0.98;
  double stateOfCharge;
  double BatteryDeltaT;

  void get_new_SOC_charge(double P_request);
  double get_new_SOC_discharge(double P_request);
  double energy_calc() const;
  double get_charge_delta() const;
  double calculateDeltaE(double P_request) const;
  void calculateStateOfCharge(double energy);
  void doAction();
};

#endif  // APPLIANCE_BATTERY_H_
