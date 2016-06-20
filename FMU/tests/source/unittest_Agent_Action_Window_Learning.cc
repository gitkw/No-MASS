// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Agent_Action_Window_Learning.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Window_Learning : public ::testing::Test {
 protected:
    Agent_Action_Window_Learning aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Window_Learning::SetUp() {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.learnep = 0.8;
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addVariable("hour");
  DataStore::addVariable("month");
  DataStore::addVariable("hourOfDay");
  DataStore::addVariable("day");
  DataStore::addValue("hour", 1);
  DataStore::addValue("month", 1);
  DataStore::addValue("hourOfDay", 1);
  DataStore::addValue("day", 1);
}

TEST_F(Test_Agent_Action_Window_Learning, Learn) {
  aal.setup(1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen(zs);
  activities.push_back(4);

  double result = 1;
  for (int i =0; i < 10; i++) {
     if (result > 0) {
       aal.setReward(1);
     } else {
       aal.setReward(-0.1);
     }

     aal.step(z_Kitchen, true, true);
     result = aal.getResult();
     DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 21);
  }

  aal.step(z_Kitchen, true, true);
  result = aal.getResult();
  ASSERT_NEAR(result, 1, 0.1);

  for (int i =0; i < 100; i++) {
     if (result > 0) {
       aal.setReward(-0.1);
     } else {
       aal.setReward(1);
     }
     aal.step(z_Kitchen, true, true);
     result = aal.getResult();
     DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 21);
  }

  aal.step(z_Kitchen, true, true);
  result = aal.getResult();
  ASSERT_NEAR(result, 0, 0.1);
}
