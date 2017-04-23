// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_CSV_H_
#define APPLIANCE_LARGE_CSV_H_

#include <vector>
#include "Appliance_Large.hpp"
#include "Model_Appliance_Large_Usage_Survival.hpp"

/**
 * @brief Large appliances class which uses CSV profiles
 * @details The large appliance agent, handles the model survival/ markov hybrid model
 *          however the profile is now take from a csv file
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
