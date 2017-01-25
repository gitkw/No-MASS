// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_BATTERY_GRIDCOST_REWARD_H_
#define APPLIANCE_BATTERY_GRIDCOST_REWARD_H_

#include <vector>
#include "QLearning_Appliance.h"
#include "Appliance_Battery.h"

/**
 * @brief Battery Appliance class
 * @details The Battery appliance agent
 */

class Appliance_Battery_GridCost_Reward : public Appliance_Battery {
 public:
  Appliance_Battery_GridCost_Reward();


  double rewardFunction(double mostShortage, double binShortage) const;
  void setGridCost(double gridCost);
 private:
  // double gridCost;

};

#endif  // APPLIANCE_BATTERY_GRIDCOST_REWARD_H_
