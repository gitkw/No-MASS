// Copyright 2016 Jacob Chapman

#include "SimulationConfig.h"
#include "Appliance_Group_FMI.h"

Appliance_Group_FMI::Appliance_Group_FMI() {}

void Appliance_Group_FMI::setup(const std::vector<applianceStruct> & app,
                                const int & buildingID,
    const std::string & buildingString) {

      for (const applianceStruct s : app) {
        appliances.push_back(Appliance_FMI());
        appliances.back().setID(s.id);
        appliances.back().setBuildingID(buildingID);
        appliances.back().setHoulyPriority(s.priority);
        appliances.back().setFMIVariableName(s.variableName);
        appliances.back().setup();
        appliances.back().setIDString(buildingString + std::to_string(s.id));
        appliances.back().setupSave();
      }
      idString = buildingString + "_FMI_Sum";
      setupSave();

}
