// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Shades_BDI.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Shades_BDI : public ::testing::Test {
 protected:
    Occupant_Action_Shades_BDI aas;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Shades_BDI::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenDaylightingReferencePoint1Illuminance", 2);
  DataStore::addValue("EnvironmentSiteExteriorHorizontalSkyIlluminance", 200);
}

TEST_F(Test_Occupant_Action_Shades_BDI, ClosedDuringSleep) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  aas.setClosedDuringSleep(true);

  activities.push_back(1);
  aas.step(z_Kitchen, true, false);
  aas.doRecipe(activities);
  EXPECT_GT(aas.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(0);
  aas.step(z_Kitchen, true, false);
  aas.doRecipe(activities);
  EXPECT_EQ(aas.getResult(), 0);
}

TEST_F(Test_Occupant_Action_Shades_BDI, ClosedDuringWashing) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  aas.setClosedDuringWashing(true);
  aas.getResult();

  activities.push_back(6);
  aas.step(z_Kitchen, true, false);
  aas.doRecipe(activities);
  EXPECT_EQ(aas.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aas.step(z_Kitchen, true, false);
  aas.doRecipe(activities);
  EXPECT_EQ(aas.getResult(), 1);
}
