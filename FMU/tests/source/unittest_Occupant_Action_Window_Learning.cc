// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Window_Learning.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Window_Learning : public ::testing::Test {
 protected:
    Occupant_Action_Window_Learning aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Window_Learning::SetUp() {
  SimulationConfig::buildings[0].agents.clear();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.learnep = 0.1;
  SimulationConfig::info.learn = 1;
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

TEST_F(Test_Occupant_Action_Window_Learning, Learn) {
  aal.setZoneId(10);
  aal.setup(1);
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;

  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
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
     z_Kitchen.setWindowState(result);
     DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 21);
  }

  aal.step(z_Kitchen, true, true);
  result = aal.getResult();
  EXPECT_NEAR(result, 1, 0.1);

  for (int i =0; i < 100; i++) {
     if (result > 0) {
       aal.setReward(-0.1);
     } else {
       aal.setReward(1);
     }
     aal.step(z_Kitchen, true, true);
     result = aal.getResult();
     z_Kitchen.setWindowState(result);
     DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 21);
  }

  aal.step(z_Kitchen, true, true);
  result = aal.getResult();
  EXPECT_NEAR(result, 1, 0.1);
}
