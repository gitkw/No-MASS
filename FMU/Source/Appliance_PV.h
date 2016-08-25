// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_PV_H_
#define APPLIANCE_PV_H_

#include <vector>
#include <string>
#include "Appliance.h"
#include "Model_Appliance_PV_CSV.h"

/**
 * @brief PV appliance class
 * @details The PV agent, handles the csv model
 */

class Appliance_PV : public Appliance {
 public:
    Appliance_PV();
    void setup();
    void preprocess();
    void setFileName(std::string filename);

 private:
    Model_Appliance_PV_CSV model;
    std::string filename;
};

#endif  // APPLIANCE_PV_H_
