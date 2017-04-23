// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Lights_BDI.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Action_Lights_BDI : public ::testing::Test {
 protected:
    Occupant_Action_Lights_BDI aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Lights_BDI::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = false;
  Configuration::info.shading = false;
  Configuration::info.lights = false;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("Block1:KitchenDaylightingReferencePoint1Illuminance");

  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 18);
  DataStore::addValueS("Block1:KitchenDaylightingReferencePoint1Illuminance", 1);
}

TEST_F(Test_Occupant_Action_Lights_BDI, OffDuringSleep) {
  ConfigStructZone zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  aal.setOffDuringSleep(true);

  activities.push_back(0);
  aal.step(z_Kitchen, true, false, activities);
  aal.doRecipe(activities);
  EXPECT_EQ(aal.getResult(), 0);

  Configuration::step();
  activities.push_back(1);
  aal.step(z_Kitchen, true, false, activities);
  aal.doRecipe(activities);
  EXPECT_EQ(aal.getResult(), 1);
}

TEST_F(Test_Occupant_Action_Lights_BDI, OffDuringAudioVisual) {
  ConfigStructZone zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  aal.setOffDuringAudioVisual(true);
  aal.getResult();

  activities.push_back(2);
  aal.step(z_Kitchen, true, false, activities);
  aal.doRecipe(activities);
  EXPECT_EQ(aal.getResult(), 0);

  Configuration::step();
  activities.push_back(1);
  aal.step(z_Kitchen, true, false, activities);
  aal.doRecipe(activities);
  EXPECT_EQ(aal.getResult(), 1);
}
