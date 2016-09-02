// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Window_Stochastic.h"
#include "Utility.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Window_Stochastic : public ::testing::Test {
 protected:
    Occupant_Action_Window_Stochastic aaw;

    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Window_Stochastic::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.timeStepsPerHour = 12;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);
  activities.clear();
}

TEST_F(Test_Occupant_Action_Window_Stochastic, Arrival) {
  Utility::setSeed(1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  z_Kitchen.setWindowState(0);
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 10);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 35);

  double previousDuration = 5*12*10*60;
  int timeStepLengthInMinutes = 5;
  for (int i =0; i < 7200; i++) {
    activities.push_back(4);
  }
  for (int i =0; i < 2; i++) {
    aaw.step(z_Kitchen, true, false, activities);
    EXPECT_FALSE(aaw.getResult());
  }

  // aaw.step(z_Kitchen, true, false, activities);
  // EXPECT_TRUE( aaw.getResult());
}
