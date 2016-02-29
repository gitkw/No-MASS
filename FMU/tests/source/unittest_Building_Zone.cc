// Copyright 2015 Jacob Chapman

#include <limits.h>


#include "Gen.h"
#include "Building_Zone.h"
#include "DataStore.h"
#include "gtest/gtest.h"

TEST(Building_Zone, Name) {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);

  ZoneStruct zs;
  zs.name = "Out";
  zs.id = 0;
  Building_Zone z_Out("", zs);

  ASSERT_EQ(z_Out.getName(), zs.name);
  ASSERT_EQ(z_Out.isActive(), false);

  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen("", zs);

  ASSERT_EQ(z_Kitchen.getName(), zs.name);
  ASSERT_EQ(z_Kitchen.isActive(), true);
  z_Kitchen.setActive(false);
  ASSERT_EQ(z_Kitchen.isActive(), false);
  z_Kitchen.setActive(true);
  ASSERT_EQ(z_Kitchen.isActive(), true);
  z_Kitchen.setCurrentAgentGains(120);
  ASSERT_EQ(z_Kitchen.getCurrentAgentGains(), 120);
  z_Kitchen.setGroundFloor(true);
  ASSERT_EQ(z_Kitchen.getGroundFloor(), true);

  // z_Kitchen.setWindowDurationOpen(300.0);
  // ASSERT_EQ(z_Kitchen.getWindowDurationOpen(), 300);

  z_Kitchen.setWindowState(true);
  ASSERT_EQ(z_Kitchen.getWindowState(), 1);
  z_Kitchen.setWindowState(false);
  ASSERT_EQ(z_Kitchen.getWindowState(), 0);
  z_Kitchen.setLightState(true);
  ASSERT_EQ(z_Kitchen.getLightState(), 1);
  z_Kitchen.setLightState(false);
  ASSERT_EQ(z_Kitchen.getLightState(), 0);
  z_Kitchen.setBlindState(0.67);
  ASSERT_EQ(z_Kitchen.getBlindState(), 0.67);
  z_Kitchen.setOccupantFraction(0.55);
  ASSERT_NEAR(z_Kitchen.getOccupantFraction(), 0.55, 0.001);
}
