// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_FMI_H_
#define APPLIANCE_FMI_H_

#include <string>
#include "Appliance.hpp"

/**
 * @brief FMI appliances class
 * @details This will handles the power coming from the FMI interface
 * for example electric heaters
 */

class Appliance_FMI : public Appliance {
 public:
    Appliance_FMI();
    virtual ~Appliance_FMI() = default;

    void setup(ConfigStructAppliance a);
    void step();
    void setFMIVariableName(const std::string & FMIVariableName);
 private:
    int FMIVariableName;
};

#endif  // APPLIANCE_FMI_H_
