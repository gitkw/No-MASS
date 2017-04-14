// Copyright 2016 Jacob Chapman

#include "Simulation.h"
#include "Appliance_Battery_GridCost_Reward.h"

Appliance_Battery_GridCost_Reward::Appliance_Battery_GridCost_Reward() {}

double Appliance_Battery_GridCost_Reward::rewardFunction(double mostShortage,
                                          double binShortage) const {
  double gridCost = Simulation::getGridCost();
  double x = binShortage / mostShortage;
  double ret = x / gridCost;
  if (action) {
    if (x > 0.7){
      ret = x * gridCost;
    } else {
      ret = -x / gridCost;
    }
  } else {
    if (x > 0.7) {
      ret = -x * gridCost;
    }
  }
  return ret;
}
