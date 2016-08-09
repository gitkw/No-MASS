// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_APPLIANCE_H_
#define FMU_SOURCE_APPLIANCE_H_

#include <vector>
#include "Agent.h"

class Appliance : public Agent {
 public:
    Appliance();
    double powerAt(const int timestep) const;
    double supplyAt(const int timestep) const;
    double getPriority() const;
    void setPriority(double priority);

 protected:
    std::vector<double> power;
    std::vector<double> supply;
    double priority;
};

#endif  // FMU_SOURCE_APPLIANCE_H_
