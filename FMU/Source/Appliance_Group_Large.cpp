// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Appliance_Group_Large.h"

Appliance_Group_Large::Appliance_Group_Large() {}

void Appliance_Group_Large::setup(const std::vector<applianceStruct> & app,
                                const int & buildingID,
                                const std::string & buildingString) {
  for (const applianceStruct &s : app) {
    appliances.push_back(Appliance_Large());
    appliances.back().setID(s.id);
    appliances.back().setBuildingID(buildingID);
    appliances.back().setHoulyPriority(s.priority);
    appliances.back().setBuildingID(buildingID);
    appliances.back().setActivities(s.activities);
    appliances.back().setup();
    appliances.back().setIDString(buildingString + std::to_string(s.id));
    appliances.back().setupSave();
  }
  idString = buildingString + "_Large_Sum";
  setupSave();
}
