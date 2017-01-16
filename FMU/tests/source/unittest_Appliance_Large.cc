// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "Gen.h"

#include "SimulationConfig.h"
#include "Appliance_Large.h"

#include "gtest/gtest.h"

class Test_Appliance_Large : public ::testing::Test {
 protected:
    Appliance_Large al;

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Large::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "SimulationConfig.xml");
  SimulationConfig::setStepCount(-1);
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";
  al.setID(1);
  al.setup();
}

void Test_Appliance_Large::AfterConfiguration() {
}

TEST_F(Test_Appliance_Large, power) {
  SimulationConfig::step();
  al.step();
  EXPECT_FALSE(al.isOn());
  EXPECT_FALSE(al.getPower());
  std::vector<int> v = {8};
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.hasActivities(v);
    al.step();
    if (al.isOn()) {
      EXPECT_GT(al.getPower(), 0);
    } else {
      EXPECT_EQ(al.getPower(), 0);
    }
  }
}
