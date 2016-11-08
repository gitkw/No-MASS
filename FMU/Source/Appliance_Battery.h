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
  double batteryPower;
  double batteryCost;
  double MaxPower;
  double chargeRate;
  double dischargeRate;
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

  double stateOfCharge;

  double Voc(double SOC);
  double Rbatt(double SOC);
  double Vter_disch(double SOC);
  double Vter_ch(double SOC);
  double P_ch(double SOC, double I);
  double P_dis(double SOC, double I);
};

#endif  // APPLIANCE_BATTERY_H_
