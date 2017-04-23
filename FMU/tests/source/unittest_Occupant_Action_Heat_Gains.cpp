// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Heat_Gains.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Action_Heat_Gains : public ::testing::Test {
 protected:
    Occupant_Action_Heat_Gains aahg;
    virtual void SetUp();
};

void Test_Occupant_Action_Heat_Gains::SetUp() {
  DataStore::clear();
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
  aahg.setup(0,0);
}

TEST_F(Test_Occupant_Action_Heat_Gains, HeatGains) {
  ConfigStructZone zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  // aahg.prestep(double clo, double metabolicRate)
  aahg.prestep(1.0, 0.1);
  aahg.step(z_Kitchen, true);
  EXPECT_NEAR(aahg.getResult(), 85.5, 0.01);
  Configuration::step();
  aahg.step(z_Kitchen, true);
  EXPECT_NEAR(aahg.getResult(), 85.5, 0.01);
}
