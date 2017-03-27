// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Appliance_Group_CSV.h"

Appliance_Group_CSV::Appliance_Group_CSV() {}

void Appliance_Group_CSV::setup(const std::vector<applianceStruct> & appCSV,
                                const int & buildingID,
                                const std::string & buildingString) {
  for (const applianceStruct s : appCSV) {
    appliances.push_back(Appliance_Generic_CSV());
    appliances.back().setID(s.id);
    appliances.back().setHoulyPriority(s.priority);
    appliances.back().setHourlyCost(s.costVector);
    appliances.back().setBuildingID(buildingID);
    appliances.back().setFileDemand(s.fileDemand);
    appliances.back().setFileSupply(s.fileSupply);
    appliances.back().setup();
    appliances.back().setIDString(buildingString + std::to_string(s.id));
    appliances.back().setupSave();
  }
  idString = buildingString + "_CSV_Sum";
  setupSave();
}
