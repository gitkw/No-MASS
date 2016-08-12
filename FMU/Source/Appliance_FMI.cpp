// Copyright 2016 Jacob Chapman

#include <string>
#include "DataStore.h"
#include "Appliance_FMI.h"

Appliance_FMI::Appliance_FMI() {
}

void Appliance_FMI::setFMIVariableName(const std::string & FMIVariableName) {
  this->FMIVariableName = FMIVariableName;
}

void Appliance_FMI::step() {
  double p = DataStore::getValue(FMIVariableName);
  power.push_back(p);
  supply.push_back(0.0);
}
