// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Window_Stochastic_BDI.h"
#include "Utility.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Window_Stochastic_BDI : public ::testing::Test {
 protected:
    Occupant_Action_Window_Stochastic_BDI aaw;

    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Window_Stochastic_BDI::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
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

TEST_F(Test_Occupant_Action_Window_Stochastic_BDI, Arrival) {
  Utility::setSeed(1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  z_Kitchen.setWindowState(0);
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 10);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 35);

  aaw.setOpenDuringCooking(false);
  aaw.setOpenDuringWashing(false);
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

TEST_F(Test_Occupant_Action_Window_Stochastic_BDI, OpenWindowDuringCooking) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  aaw.setOpenDuringCooking(true);

  activities.push_back(4);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 0);
}

TEST_F(Test_Occupant_Action_Window_Stochastic_BDI, OpenWindowAfterShower) {
  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  aaw.setOpenDuringWashing(true);
  aaw.getResult();

  activities.push_back(6);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(6);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_EQ(aaw.getResult(), 0);
}



TEST_F(Test_Occupant_Action_Window_Stochastic_BDI, OpenWindowAfterShower2) {
  DataStore::addVariable("Block1:BathZoneMeanAirTemperature");
  DataStore::addVariable("Block1:BathZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:BathZoneMeanRadiantTemperature");

  DataStore::addValue("Block1:BathZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:BathZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:BathZoneMeanRadiantTemperature", 18);


  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  zs.activities = {1, 2, 3};
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  zs.name = "Block1:Bath";
  zs.id = 2;
  zs.activities = {6};
  Building_Zone z_Bath;
  z_Bath.setName(zs.name);
  z_Bath.setup(zs);

  Occupant_Action_Window_Stochastic_BDI aab;
  if (z_Kitchen.hasActivity(6)) {
    aaw.setOpenDuringWashing(true);
  }
  if (z_Bath.hasActivity(6)) {
    aab.setOpenDuringWashing(true);
  }

  aaw.getResult();
  aab.getResult();

  activities.push_back(6);
  aaw.step(z_Kitchen, true, false, activities);
  aaw.doRecipe(activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_FALSE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(6);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_FALSE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_TRUE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_TRUE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 0);


  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_FALSE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
  aab.step(z_Bath, true, false, activities);
  ASSERT_FALSE(aab.doRecipe(activities));
  ASSERT_EQ(aab.getResult(), 0);
}

TEST_F(Test_Occupant_Action_Window_Stochastic_BDI, multiZone) {
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 23);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 26);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 50);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 23);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  zs.activities = {1, 2, 3};
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);

  aaw.getResult();

  for (int i = 0; i < 100; i++) {
    activities.push_back(0);
    aaw.step(z_Kitchen, false, false, activities);
    z_Kitchen.setWindowState(aaw.getResult());
    ASSERT_FALSE(aaw.doRecipe(activities));
    ASSERT_EQ(aaw.getResult(), 0);
  }

  while (aaw.getResult() == 0 || aaw.durationOpen() < 1000) {
    SimulationConfig::step();
    activities.push_back(1);
    aaw.step(z_Kitchen, true, false, activities);
    z_Kitchen.setWindowState(aaw.getResult());
    ASSERT_FALSE(aaw.doRecipe(activities));
  }
  ASSERT_EQ(aaw.getResult(), 1);
  ASSERT_EQ(aaw.durationOpen(), 1007);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, true, activities);
  z_Kitchen.setWindowState(aaw.getResult());
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.durationOpen(), 1002);
  ASSERT_EQ(aaw.getResult(), 1);

  SimulationConfig::step();
  activities.push_back(2);
  aaw.step(z_Kitchen, true, true, activities);
  z_Kitchen.setWindowState(aaw.getResult());
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.durationOpen(), 997);
  ASSERT_EQ(aaw.getResult(), 1);
  int prevDuration =  aaw.durationOpen();
  while (aaw.durationOpen() > 0) {
    SimulationConfig::step();
    activities.push_back(2);
    aaw.step(z_Kitchen, true, true, activities);
    z_Kitchen.setWindowState(aaw.getResult());
    ASSERT_FALSE(aaw.doRecipe(activities));
    prevDuration = prevDuration - 5;
    if (prevDuration < 0 ) prevDuration = 0;
    ASSERT_EQ(aaw.durationOpen(), prevDuration);
    if (prevDuration > 0) {
      ASSERT_EQ(aaw.getResult(), 1);
    } else {
      ASSERT_EQ(aaw.getResult(), 0);
    }
  }

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);

  SimulationConfig::step();
  activities.push_back(1);
  aaw.step(z_Kitchen, true, false, activities);
  ASSERT_FALSE(aaw.doRecipe(activities));
  ASSERT_EQ(aaw.getResult(), 0);
}
