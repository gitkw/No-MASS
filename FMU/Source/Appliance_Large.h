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

    void setup(applianceStruct a);
    virtual void setupModel();
    void step();
    bool isOn() const;

 protected:
    Model_Appliance_Large_Usage_Survival model;


 private:
};

#endif  // APPLIANCE_LARGE_H_
