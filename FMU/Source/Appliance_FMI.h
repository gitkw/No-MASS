// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_FMI_H_
#define APPLIANCE_FMI_H_

#include <string>
#include "Appliance.h"

class Appliance_FMI : public Appliance {
 public:
    Appliance_FMI();


    void step();
    void setFMIVariableName(const std::string & FMIVariableName);
 private:
    std::string FMIVariableName;
};

#endif  // APPLIANCE_FMI_H_
