// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Lights.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Lights : public ::testing::Test {
 protected:
    Agent_Action_Lights aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Lights::SetUp() {
  SimulationConfig::reset();
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

TEST_F(Test_Agent_Action_Lights, OffDuringSleep) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  Zone z_Kitchen("", zs);
  aal.setOffDuringSleep(true);

  activities.push_back(0);
  aal.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aal.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aal.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aal.getResult(), 1);
}

TEST_F(Test_Agent_Action_Lights, OffDuringAudioVisual) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  Zone z_Kitchen("", zs);
  aal.setOffDuringAudioVisual(true);
  aal.getResult();

  activities.push_back(2);
  aal.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aal.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aal.step(z_Kitchen, true, false, activities);
  ASSERT_EQ(aal.getResult(), 1);
}
