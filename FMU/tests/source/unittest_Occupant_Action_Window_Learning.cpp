// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Window_Learning.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Action_Window_Learning : public ::testing::Test {
 protected:
    Occupant_Action_Window_Learning aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Window_Learning::SetUp() {
  Configuration::buildings[0].agents.clear();
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = false;
  Configuration::info.shading = false;
  Configuration::info.lights = false;
  Configuration::info.learnep = 0.0;
  Configuration::info.learn = 1;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addVariable("hour");
  DataStore::addVariable("month");
  DataStore::addVariable("hourOfDay");
  DataStore::addVariable("day");
  DataStore::addValueS("hour", 1);
  DataStore::addValueS("month", 1);
  DataStore::addValueS("hourOfDay", 1);
  DataStore::addValueS("day", 1);
}

TEST_F(Test_Occupant_Action_Window_Learning, Learn) {
  aal.setZoneId(10);
  aal.setup(1);
  ConfigStructZone zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;

  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
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
     DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 21);
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
     DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 21);
  }

  aal.step(z_Kitchen, true, true);
  result = aal.getResult();
  EXPECT_NEAR(result, 1, 0.1);
}
