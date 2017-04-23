// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_H_
#define APPLIANCE_LARGE_H_

#include <vector>
#include "Appliance.hpp"
#include "Model_Appliance_Large_Usage_Survival.hpp"

/**
 * @brief Large appliances class
 * @details The large appliance agent, handles the model survival/ markov hybrid model
 */

class Appliance_Large : public Appliance {
 public:
    Appliance_Large();

    void setup(ConfigStructAppliance a);
    virtual void setupModel();
    void step();
    bool isOn() const;

 protected:
   void setFile(std::string file);
     std::string file;
     std::vector<double> profileCSV;
    Model_Appliance_Large_Usage_Survival model;


 private:
};

#endif  // APPLIANCE_LARGE_H_
