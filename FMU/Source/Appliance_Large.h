// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_H_
#define APPLIANCE_LARGE_H_

#include <vector>
#include "Appliance.h"
#include "Model_Appliance_Large_Usage_Survival.h"

/**
 * @brief Large appliances class
 * @details The large appliance agent, handles the model survival/ markov hybrid model
 */

class Appliance_Large : public Appliance {
 public:
    Appliance_Large();

    void setup();
    void setupModel();
    void step();
    void setActivities(const std::vector<int> Activities);
    bool hasActivities(const std::vector<int> Activities);
    bool isOn() const;

 protected:
    Model_Appliance_Large_Usage_Survival model;
    bool match;

 private:
    std::vector<int> Activities;
};

#endif  // APPLIANCE_LARGE_H_
