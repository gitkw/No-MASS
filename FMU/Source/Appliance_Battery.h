// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_BATTERY_H_
#define APPLIANCE_BATTERY_H_

#include <vector>

#include "QLearning_Appliance.h"
#include "Appliance.h"

/**
 * @brief Large appliances class
 * @details The large appliance agent, handles the model survival/ markov hybrid model
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

  double batteryPower;
  double batteryCost;
  double MaxPower;
  double chargeRate = 1000;
  double dischargeRate = 1000;
  double powerShortage;
  QLearning_Appliance qLearning;
  double epsilon;   // probability of a random action selection
  double alpha;     // learning rate
  double gamma;     // discount factor (how soon do you care)
  bool update;

  bool action;
  double binShortage;
  double mostShortage;
  double cost;
  double delta_E;
  double energy;
  //double I = 30.0;
  double capacity = 2000;
  double efficiency = 0.98;

  double stateOfCharge;

  //double Voc(double SOC);
  //double Rbatt(double SOC);
  //double Vter_disch(double SOC);
  //double Vter_ch(double SOC);
  //double P_ch(double SOC);
  //double P_dis(double SOC);

  void get_new_SOC_charge(double delta_E, double P_request);
  double get_new_SOC_discharge(double delta_t, double P_request);
  double energy_calc(double SOC, double capacity);
  double get_charge_delta();
};

#endif  // APPLIANCE_BATTERY_H_
