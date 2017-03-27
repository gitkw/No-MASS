// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Appliance_Group_Small.h"

Appliance_Group_Small::Appliance_Group_Small() {}

void Appliance_Group_Small::setup(const std::vector<applianceStruct> & appSmall,
                                const int & buildingID,
                                const std::string & buildingString) {
  for (const applianceStruct s : appSmall) {
    appliances.push_back(Appliance_Small());
    appliances.back().setID(s.id);
    appliances.back().setBuildingID(buildingID);
    appliances.back().setHoulyPriority(s.priority);
    appliances.back().setWeibullParameters(s.WeibullParameters);
    appliances.back().setStateProbabilities(s.StateProbabilities);
    appliances.back().setFractions(s.Fractions);
    appliances.back().setSumRatedPowers(s.SumRatedPowers);
    appliances.back().setup();
    appliances.back().setIDString(buildingString + std::to_string(s.id));
    appliances.back().setupSave();
  }
  idString = buildingString + "_Small_Sum";
  setupSave();
}
