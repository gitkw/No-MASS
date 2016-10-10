// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "Gen.h"
#include "SimulationConfig.h"
#include "Building_Appliances.h"
#include "gtest/gtest.h"

class Test_Building_Appliances : public ::testing::Test {
 protected:
    Building_Appliances appliances;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Building_Appliances::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(SimulationConfig::RunLocation +
                                                  "SimulationConfig.xml");
  SimulationConfig::setStepCount(-1);
  buildingStruct b = SimulationConfig::buildings[0];
  appliances.setup(b);
  appliances.preprocess();
}

void Test_Building_Appliances::AfterConfiguration() {}

TEST_F(Test_Building_Appliances, power) {
  SimulationConfig::step();
  appliances.addCurrentStates(8);
  appliances.stepLocal();

  for (int i = 1; i < 10000; i++) {
    SimulationConfig::step();
    appliances.addCurrentStates(8);
      appliances.stepLocal();
      appliances.stepLocalNegotiation();
      appliances.postTimeStep();
  }
}
