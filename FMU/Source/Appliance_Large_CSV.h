// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_CSV_H_
#define APPLIANCE_LARGE_CSV_H_

#include <vector>
#include "Appliance_Large.h"
#include "Model_Appliance_Large_Usage_Survival.h"

/**
 * @brief Large appliances class
 * @details The large appliance agent, handles the model survival/ markov hybrid model
 */

class Appliance_Large_CSV : public Appliance_Large {
 public:
    Appliance_Large_CSV();
    void step();

 protected:

    void setupModel();
    unsigned int count;
    bool running;

 private:
};

#endif  // APPLIANCE_LARGE_CSV_H_
