// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Appliance_Group_Large_Learning.h"

Appliance_Group_Large_Learning::Appliance_Group_Large_Learning() {}

void Appliance_Group_Large_Learning::setup(const std::vector<appLargeStruct> & app,
                                const int & buildingID,
                                const std::string & buildingString) {

    for (const appLargeStruct &s : app) {
      appliances.push_back(Appliance_Large_Learning());
      appliances.back().setEpsilon(s.epsilon);
      appliances.back().setAlpha(s.alpha);
      appliances.back().setGamma(s.gamma);
      appliances.back().setUpdate(s.update);
      appliances.back().setID(s.id);
      appliances.back().setHoulyPriority(s.priority);
      appliances.back().setHoulyTimeRequired(s.timeRequired);
      appliances.back().setBuildingID(buildingID);
      appliances.back().setActivities(s.activities);
      appliances.back().setup();
      appliances.back().setIDString(buildingString + std::to_string(s.id));
      appliances.back().setupSave();
    }
    idString = buildingString + "_LargeLearning_Sum";
    setupSave();
}
