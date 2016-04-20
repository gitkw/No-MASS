// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"
#include "Utility.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Window : public ::testing::Test {
 protected:
    Agent_Action_Window aaw;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Window::SetUp() {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);
}

TEST_F(Test_Agent_Action_Window, Arrival) {
  Utility::setSeed(1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen(zs);
  z_Kitchen.setWindowState(0);
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 10);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 35);

  aaw.setOpenDuringCooking(false);
  aaw.setOpenDuringWashing(false);
  double previousDuration = 5*12*10*60;
  int timeStepLengthInMinutes = 5;
  for (int i =0; i < 7200; i++) {
    activities.push_back(4);
  }
  for (int i =0; i < 2; i++) {
    aaw.step(z_Kitchen, true, false, activities);
    EXPECT_FALSE(aaw.getResult());
  }

  // aaw.step(z_Kitchen, true, false, activities);
  // EXPECT_TRUE( aaw.getResult());
}

TEST_F(Test_Agent_Action_Window, OpenWindowDuringCooking) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen(zs);
  aaw.setOpenDuringCooking(true);

  activities.push_back(4);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 0);
}

TEST_F(Test_Agent_Action_Window, OpenWindowAfterShower) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen(zs);
  aaw.setOpenDuringWashing(true);
  aaw.getResult();

  activities.push_back(6);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 0);
}
