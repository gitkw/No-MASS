// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "tests/Gen.h"

#include "SimulationConfig.h"
#include "Appliance_Large.h"

#include "gtest/gtest.h"

class Test_Appliance_Large : public ::testing::Test {
 protected:


    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Large::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "SimulationConfig.xml");
  SimulationConfig::setStepCount(-1);
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";

}

void Test_Appliance_Large::AfterConfiguration() {
}

TEST_F(Test_Appliance_Large, power) {
  applianceStruct a;
    a.id = 1;

  Appliance_Large al;
  al.setup(a);

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



TEST_F(Test_Appliance_Large, trueFalse) {
    applianceStruct a;
    a.id = 3;
  Appliance_Large al;
    al.setup(a);
    
  SimulationConfig::step();
  al.step();
  EXPECT_FALSE(al.isOn());
  EXPECT_FALSE(al.getPower());
  bool on = false;
  bool off = false;
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.step();
    if (al.isOn()) {
      EXPECT_GT(al.getPower(), 0);
      on = true;
    } else {
      EXPECT_EQ(al.getPower(), 0);
      off = true;
    }
  }
  EXPECT_TRUE(on);
  EXPECT_TRUE(off);
}
