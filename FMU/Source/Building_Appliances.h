// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_BUILDING_APPLIANCES_H_
#define FMU_SOURCE_BUILDING_APPLIANCES_H_

#include <vector>
#include "Appliance_Large.h"
#include "Appliance_Small.h"


class Building_Appliances {
 public:
    Building_Appliances();

    void setup();
    void step();
    void postprocess();
    void preprocess();
    void postTimeStep();

    void setBuildingID(const int id);

 private:
  std::vector<Appliance_Large> large;
  std::vector<Appliance_Small> small;
  int buildingID;
};

#endif  // FMU_SOURCE_BUILDING_APPLIANCES_H_
