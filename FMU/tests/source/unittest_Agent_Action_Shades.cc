// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Shades.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Shades : public ::testing::Test {
 protected:
    Agent_Action_Shades aas;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Shades::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenDaylightingReferencePoint1Illuminance", 2);
  DataStore::addValue("EnvironmentSiteExteriorHorizontalSkyIlluminance", 200);
}

TEST_F(Test_Agent_Action_Shades, ClosedDuringSleep) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen("", zs);
  aas.setClosedDuringSleep(true);

  activities.push_back(0);
  aas.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aas.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aas.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aas.getResult(), 1);
}

TEST_F(Test_Agent_Action_Shades, ClosedDuringWashing) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen("", zs);
  aas.setClosedDuringWashing(true);
  aas.getResult();

  activities.push_back(6);
  aas.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aas.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aas.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aas.getResult(), 1);
}
