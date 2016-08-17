// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_LEARNING_H_
#define APPLIANCE_LARGE_LEARNING_H_

#include <vector>
#include "QLearning_Appliance.h"
#include "Appliance_Large.h"

/**
 * @brief Large appliances learning class
 * @details This will handle the appliance learning model for profile shifting
 */

class Appliance_Large_Learning : public Appliance_Large {
 public:
    Appliance_Large_Learning();
    void setup();
    void step();
    void postprocess();
    void addToCost(const double cost);

 private:
      std::vector<double> powerProfile;
      double cost;
      bool learning;
      int startTime;
      unsigned int learningStep;
      QLearning_Appliance qLearning;
      int x;
      void stepOff();
      void getProfile();
      void getStart();
};

#endif  // APPLIANCE_LARGE_LEARNING_H_
