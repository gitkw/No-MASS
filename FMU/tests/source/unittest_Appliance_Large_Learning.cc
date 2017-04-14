// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "tests/Gen.h"

#include "SimulationConfig.h"
#include "Appliance_Large_Learning.h"

#include "gtest/gtest.h"

class Test_Appliance_Large_Learning : public ::testing::Test {
 protected:
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Large_Learning::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "SimulationConfig.xml");
  SimulationConfig::setStepCount(-1);
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";

}

void Test_Appliance_Large_Learning::AfterConfiguration() {
}

TEST_F(Test_Appliance_Large_Learning, power) {
  applianceStruct a;
    a.id = 3;
    a.timeRequired = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Appliance_Large_Learning al;
  
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
    EXPECT_TRUE(al.getPower() >= 0);


  }
}

TEST_F(Test_Appliance_Large_Learning, trueFalse) {
    applianceStruct a;
    a.id = 1;
    a.timeRequired = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  Appliance_Large_Learning al;
  al.setup(a);
  SimulationConfig::step();
  al.step();
  EXPECT_FALSE(al.isOn());
  EXPECT_FALSE(al.getPower());
  bool on = false;
  bool off = false;
  for (int i = 1; i < 100000; i++) {
    al.hasActivities({});
    SimulationConfig::step();
    al.step();
    if (al.getPower() > 0) {
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


TEST_F(Test_Appliance_Large_Learning, On) {
  SimulationConfig::info.timeStepsPerHour = 6;
  int sims = 10000;
  double onWashingMachine[144] = {0.03767,0.0236,0.02125,0.01776,0.01685,0.01516,0.01444,0.01334,0.01211,0.01111,0.01061,0.01563,0.01497,0.01393,0.01321,0.01262,0.01227,0.01356,0.01346,0.01296,0.0129,0.01171,0.01007,0.00907,0.0081,0.00813,0.00734,0.00694,0.00672,0.00694,0.007,0.00741,0.00797,0.00832,0.00985,0.01139,0.01293,0.01622,0.01983,0.02341,0.02931,0.03643,0.04651,0.05514,0.06396,0.07306,0.08172,0.09063,0.09826,0.10158,0.10585,0.10704,0.10902,0.10999,0.11222,0.11533,0.11774,0.11875,0.1195,0.11894,0.11868,0.11614,0.11523,0.11423,0.11247,0.11147,0.10993,0.1082,0.10566,0.10497,0.10328,0.10513,0.10745,0.10858,0.10776,0.10528,0.10572,0.10337,0.1045,0.1051,0.10538,0.10601,0.10466,0.10208,0.09813,0.09418,0.08887,0.08398,0.08024,0.0783,0.07557,0.07296,0.06938,0.06772,0.06643,0.06502,0.06217,0.06163,0.05975,0.05997,0.06104,0.06248,0.06116,0.06339,0.06443,0.06493,0.06593,0.06643,0.06772,0.06766,0.07058,0.07202,0.07249,0.07378,0.07387,0.07346,0.07371,0.0735,0.07312,0.07215,0.07114,0.06898,0.06756,0.06794,0.0681,0.06719,0.06738,0.06439,0.06198,0.06035,0.0568,0.05391,0.05034,0.0482,0.04742,0.04541,0.04488,0.0434,0.04161,0.03926,0.03574,0.03235,0.03082,0.04121};
    applianceStruct a;
    a.id = 1;
    a.timeRequired = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  Appliance_Large_Learning al;
  al.setup(a);

  SimulationConfig::step();

  double on[144] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  for (int i = 0; i < sims; i++) {
    for (int timeStep = 0; timeStep < 144; timeStep++) {
      al.hasActivities({});
      SimulationConfig::step();
      al.step();
      on[timeStep] += al.isModelOn();
    }
  }

  for (int timeStep = 0; timeStep < 144; timeStep++) {
    //EXPECT_NEAR(on[timeStep] / sims, onWashingMachine[timeStep], 0.004);
  }
}
