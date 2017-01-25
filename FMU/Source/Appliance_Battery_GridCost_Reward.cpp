// Copyright 2016 Jacob Chapman

#include <vector>
#include <string>
#include "SimulationConfig.h"
#include "Simulation.h"
#include "DataStore.h"
#include "Utility.h"
#include "Appliance_Battery_GridCost_Reward.h"

Appliance_Battery_GridCost_Reward::Appliance_Battery_GridCost_Reward() {
}

double Appliance_Battery_GridCost_Reward::rewardFunction(double mostShortage, double binShortage) const {
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
/*
void Appliance_Battery_GridCost_Reward::setGridCost(double gridCost){
  this->gridCost = gridCost;
}
*/
