// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GRID_H_
#define APPLIANCE_GRID_H_

#include "Appliance.h"

/**
 * @brief Grid appliances class
 * @details This will handles the power coming from the grid
 */

class Appliance_Grid : public Appliance {
 public:
  Appliance_Grid();
  void step();
  void setRequiredPower(const double power);
 private:
  double requiredPower;
};

#endif  // APPLIANCE_GRID_H_
