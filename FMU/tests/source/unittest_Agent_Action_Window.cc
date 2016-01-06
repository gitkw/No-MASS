// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Window.h"
#include "gtest/gtest.h"

TEST(Agent_Action_Window, OpenWindowDuringCooking) {
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;


  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  Zone z_Kitchen("", zs);

  std::vector<double> activities;

  Agent_Action_Window aaw;
  aaw.setOpenDuringCooking(true);

  activities.push_back(4);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aaw.getResult(), 0);
}

TEST(Agent_Action_Window, OpenWindowAfterShower) {
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  Zone z_Kitchen("", zs);

  std::vector<double> activities;

  Agent_Action_Window aaw;
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
