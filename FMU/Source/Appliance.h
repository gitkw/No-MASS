// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_APPLIANCE_H_
#define FMU_SOURCE_APPLIANCE_H_

#include <vector>
#include "Agent.h"

class Appliance : public Agent {
 public:
    Appliance();
    double powerAt(const int timestep) const;
 protected:
    std::vector<double> power;
};

#endif  // FMU_SOURCE_APPLIANCE_H_
