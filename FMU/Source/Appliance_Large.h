// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_APPLIANCE_LARGE_H_
#define FMU_SOURCE_APPLIANCE_LARGE_H_

#include <vector>
#include "Appliance.h"
#include "Model_Appliance_Large_Usage.h"

class Appliance_Large : public Appliance {
 public:
    Appliance_Large();

    void setup();
    void preprocess();

 private:
    Model_Appliance_Large_Usage model;
};

#endif  // FMU_SOURCE_APPLIANCE_LARGE_H_
