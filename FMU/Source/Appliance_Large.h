// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_H_
#define APPLIANCE_LARGE_H_

#include <vector>
#include "Appliance.h"
#include "Model_Appliance_Large_Usage_Survival.h"

class Appliance_Large : public Appliance {
 public:
    Appliance_Large();

    void setup();
    void step();
    void setActivities(const std::vector<int> Activities);
    bool hasActivities(const std::vector<int> Activities);
    bool isOn() const;
 private:
    Model_Appliance_Large_Usage_Survival model;
    std::vector<int> Activities;
    bool match;
};

#endif  // APPLIANCE_LARGE_H_
