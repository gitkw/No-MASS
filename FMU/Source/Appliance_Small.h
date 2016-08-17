// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_SMALL_H_
#define APPLIANCE_SMALL_H_

#include <vector>
#include <string>
#include "Appliance.h"
#include "Model_Appliance_Small_Usage.h"

/**
 * @brief Small appliances class
 * @details The small appliance agent, handles the model
 */

class Appliance_Small : public Appliance {
 public:
    Appliance_Small();

    void setup();
    void preprocess();
    void setWeibullParameters(const std::string & WeibullParameters);
    void setStateProbabilities(const std::string & StateProbabilities);
    void setFractions(const std::string & Fractions);
    void setSumRatedPowers(const std::string & SumRatedPowers);

 private:
    Model_Appliance_Small_Usage model;
    std::string WeibullParameters;
    std::string StateProbabilities;
    std::string Fractions;
    std::string SumRatedPowers;
};

#endif  // APPLIANCE_SMALL_H_
