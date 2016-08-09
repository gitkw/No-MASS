// Copyright 2016 Jacob Chapman

#ifndef BUILDING_APPLIANCES_H_
#define BUILDING_APPLIANCES_H_

#include <vector>
#include <string>
#include "Appliance_Large.h"
#include "Appliance_Small.h"
#include "Appliance_PV.h"
#include "LVN_Negotiation.h"


class Building_Appliances {
 public:
    Building_Appliances();

    void setup();
    void step();
    void stepLarge();
    void postprocess();
    void preprocess();
    void postTimeStep();

    void setBuildingID(const int id);

    double getTotalPower() const;
    void sendContract(int id, double priority, double request, double supply);

    void addCurrentStates(const int stateid);

 private:
  std::vector<Appliance_Large> large;
  std::vector<Appliance_Small> small;
  std::vector<Appliance_PV> pv;
  std::vector<int> currentStates;
  int buildingID;
  double totalPower;
  LVN_Negotiation app_negotiation;

  std::string buildingString;
};

#endif  // BUILDING_APPLIANCES_H_
