// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "Building_Appliances.hpp"
#include "gtest/gtest.h"

class Test_Building_Appliances : public ::testing::Test {
 protected:
    Building_Appliances appliances;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Building_Appliances::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles +
                                                  "SimulationConfig.xml");
  Configuration::setStepCount(-1);
  ConfigStructBuilding b = Configuration::buildings[0];
  appliances.setup(b);
  appliances.preprocess();
}

void Test_Building_Appliances::AfterConfiguration() {}

TEST_F(Test_Building_Appliances, power) {
  Configuration::step();
  appliances.addCurrentStates(8);
  appliances.stepLocal();

  for (int i = 1; i < 10000; i++) {
    Configuration::step();
    appliances.addCurrentStates(8);
      appliances.stepLocal();
      appliances.stepLocalNegotiation();
      appliances.postTimeStep();
  }
}
