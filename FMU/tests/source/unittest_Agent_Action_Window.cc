// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Window.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Window : public ::testing::Test {
 protected:
    Agent_Action_Window aaw;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Window::SetUp() {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);

}

TEST_F(Test_Agent_Action_Window, OpenWindowDuringCooking) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  Zone z_Kitchen("", zs);
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
  Zone z_Kitchen("", zs);
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
