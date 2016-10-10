// Copyright 2015 Jacob Chapman

#include <limits.h>


#include "Gen.h"
#include "Building_Zone.h"
#include "DataStore.h"
#include "gtest/gtest.h"

TEST(Building_Zone, Name) {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 18);

  ZoneStruct zs;
  zs.name = "out";
  zs.id = 0;
  Building_Zone z_Out;
  z_Out.setActive(false);
  z_Out.setName(zs.name);
  z_Out.setup(zs);

  EXPECT_TRUE(z_Out.isNamed(zs.name));
  EXPECT_FALSE(z_Out.isActive());

  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  z_Kitchen.setActive(true);

  EXPECT_TRUE(z_Kitchen.isNamed(zs.name));
  EXPECT_TRUE(z_Kitchen.isActive());
  z_Kitchen.setActive(false);
  EXPECT_FALSE(z_Kitchen.isActive());
  z_Kitchen.setActive(true);
  EXPECT_TRUE(z_Kitchen.isActive());
  z_Kitchen.setCurrentOccupantGains(120);
  EXPECT_EQ(z_Kitchen.getCurrentOccupantGains(), 120);
  z_Kitchen.setGroundFloor(true);
  EXPECT_TRUE(z_Kitchen.getGroundFloor());

  // z_Kitchen.setWindowDurationOpen(300.0);
  // EXPECT_EQ(z_Kitchen.getWindowDurationOpen(), 300);

  z_Kitchen.setWindowState(true);
  EXPECT_EQ(z_Kitchen.getWindowState(), 1);
  z_Kitchen.setWindowState(false);
  EXPECT_EQ(z_Kitchen.getWindowState(), 0);
  z_Kitchen.setLightState(true);
  EXPECT_EQ(z_Kitchen.getLightState(), 1);
  z_Kitchen.setLightState(false);
  EXPECT_EQ(z_Kitchen.getLightState(), 0);
  z_Kitchen.setBlindState(0.67);
  EXPECT_EQ(z_Kitchen.getBlindState(), 0.67);
  z_Kitchen.setOccupantFraction(0.55);
  EXPECT_NEAR(z_Kitchen.getOccupantFraction(), 0.55, 0.001);
}
