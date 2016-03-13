// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Heat_Gains.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Heat_Gains : public ::testing::Test {
 protected:
    Agent_Action_Heat_Gains aahg;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Heat_Gains::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("Block1:KitchenDaylightingReferencePoint1Illuminance");

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);
  DataStore::addValue("Block1:KitchenDaylightingReferencePoint1Illuminance", 1);
}

TEST_F(Test_Agent_Action_Heat_Gains, OffDuringSleep) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen("", zs);
  // aahg.prestep(double clo, double metabolicRate)
  aahg.prestep(1.0, 0.1);

  activities.push_back(0);
  aahg.step(z_Kitchen, true, false, activities);
  ASSERT_NEAR(aahg.getResult(), 85.5, 0.01);

  SimulationConfig::step();
  activities.push_back(1);
  aahg.step(z_Kitchen, true, false, activities);
  ASSERT_NEAR(aahg.getResult(), 85.5, 0.01);
}
