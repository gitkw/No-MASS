// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_LEARNING_H_
#define APPLIANCE_LARGE_LEARNING_H_

#include <vector>
#include "Appliance_Large.h"

class Appliance_Large_Learning : public Appliance_Large {
 public:
    Appliance_Large_Learning();
    void setup();
    void step();
 private:
};

#endif  // APPLIANCE_LARGE_LEARNING_H_
