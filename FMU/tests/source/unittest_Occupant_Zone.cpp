// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "tests/Gen.hpp"
#include "Occupant_Zone.hpp"
#include "DataStore.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Zone : public ::testing::Test {
 protected:
    Building_Zone z_Kitchen;
    Building_Zone z_LivingRoom;
    Occupant_Zone az;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Zone::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = true;
  Configuration::info.shading = false;
  Configuration::info.lights = false;
  Configuration::info.timeStepsPerHour = 12;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");

  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 18);
  activities.clear();

  ConfigStructZone zsk;
  zsk.name = "Block1:Kitchen";
  zsk.id = 1;
  z_Kitchen.setName(zsk.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zsk);

  ConfigStructZone zsl;
  zsl.name = "Block1:LivingRoom";
  zsl.id = 2;
  z_LivingRoom.setName(zsl.name);
  z_LivingRoom.setActive(true);
  z_LivingRoom.setup(zsl);
  ConfigStructAgent agent = Configuration::buildings[0].agents.at(1);
  az.setup(0, z_Kitchen, 1, agent);
}


TEST_F(Test_Occupant_Zone, oneZone) {
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 23);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 26);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 50);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 23);

  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  Configuration::step();
  az.step(z_Kitchen, z_Kitchen, activities);
  EXPECT_FALSE(az.getDesiredWindowState());
  az.postTimeStep();

  while (!az.getDesiredWindowState() ||
          az.getDesiredWindowDuration() < 1000) {
      Configuration::step();
      activities.push_back(6);
      az.step(z_Kitchen, z_Kitchen, activities);
      z_Kitchen.setWindowState(az.getDesiredWindowState());
      az.postTimeStep();
  }
  int previousDuration = az.getDesiredWindowDuration();
  EXPECT_GT(previousDuration, 995);
  while (az.getDesiredWindowDuration() > 0) {
    Configuration::step();
    activities.push_back(6);
    az.step(z_Kitchen, z_Kitchen, activities);
    z_Kitchen.setWindowState(az.getDesiredWindowState());
    previousDuration = previousDuration - 5;
    if (previousDuration < 0) previousDuration = 0;
    if (previousDuration > 0) {
      EXPECT_TRUE(az.getDesiredWindowState());
      EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration);
    } else {
      EXPECT_FALSE(az.getDesiredWindowState());
      EXPECT_EQ(az.getDesiredWindowDuration(), 0);
    }
    az.postTimeStep();
  }
}


TEST_F(Test_Occupant_Zone, twoZone) {
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 23);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 26);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 50);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 23);

  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  Configuration::step();
  az.step(z_Kitchen, z_LivingRoom, activities);

  EXPECT_FALSE(az.getDesiredWindowState());
  az.postTimeStep();

  while (!az.getDesiredWindowState() ||
          az.getDesiredWindowDuration() < 1000) {
      Configuration::step();
      activities.push_back(6);
      az.step(z_Kitchen, z_Kitchen, activities);
      z_Kitchen.setWindowState(az.getDesiredWindowState());
      az.postTimeStep();
  }
  int previousDuration = az.getDesiredWindowDuration();

  EXPECT_GT(previousDuration, 995);

  Configuration::step();
  activities.push_back(6);
  az.step(z_LivingRoom, z_Kitchen, activities);
  z_Kitchen.setWindowState(az.getDesiredWindowState());
  EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration);
  EXPECT_GT(az.getDesiredWindowDuration() , 995);
  EXPECT_TRUE(az.getDesiredWindowState());
  az.postTimeStep();

  for (int i = 0; i < 10000; i++) {
    az.step(z_LivingRoom, z_LivingRoom, activities);
    EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration);
    EXPECT_GT(az.getDesiredWindowDuration() , 995);
    EXPECT_TRUE(az.getDesiredWindowState());
    az.postTimeStep();
  }
  az.step(z_Kitchen, z_LivingRoom, activities);
  z_Kitchen.setWindowState(az.getDesiredWindowState());
  previousDuration = az.getDesiredWindowDuration();
  EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration);
  EXPECT_TRUE(az.getDesiredWindowState());
  az.postTimeStep();

  az.step(z_Kitchen, z_Kitchen, activities);
  z_Kitchen.setWindowState(az.getDesiredWindowState());
  EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration - 5);
  EXPECT_TRUE(az.getDesiredWindowState());
  az.postTimeStep();

  z_Kitchen.setWindowState(0);
  az.step(z_Kitchen, z_Kitchen, activities);
  EXPECT_EQ(az.getDesiredWindowDuration(), 0);
  EXPECT_FALSE(az.getDesiredWindowState());
  az.postTimeStep();

  while (!az.getDesiredWindowState() ||
          az.getDesiredWindowDuration() < 1000) {
    z_Kitchen.setWindowState(0);
    az.step(z_Kitchen, z_Kitchen, activities);
    az.postTimeStep();
  }
  previousDuration = az.getDesiredWindowDuration();
  EXPECT_EQ(az.getDesiredWindowDuration(), previousDuration);
  EXPECT_TRUE(az.getDesiredWindowState());
  az.postTimeStep();
  z_Kitchen.setWindowState(0);
  az.step(z_Kitchen, z_Kitchen, activities);
  EXPECT_FALSE(az.getDesiredWindowState());
  az.postTimeStep();
  z_Kitchen.setWindowState(1);
  az.step(z_Kitchen, z_Kitchen, activities);
  EXPECT_FALSE(az.getDesiredWindowState());
  az.postTimeStep();

  for (int i = 0; i < 100000; i++) {
    int first = az.getDesiredWindowDuration();
    z_Kitchen.setWindowState(0);
    az.step(z_Kitchen, z_Kitchen, activities);
    az.postTimeStep();
    z_Kitchen.setWindowState(1);
    az.step(z_Kitchen, z_Kitchen, activities);
    int second = az.getDesiredWindowDuration();
    az.postTimeStep();
    if (first > 0 && second > 0) {
      EXPECT_FALSE(first == second);
    }
  }
  for (int i = 0; i < 100000; i++) {
    int first = az.getDesiredWindowDuration();
    z_Kitchen.setWindowState(0);
    az.step(z_Kitchen, z_LivingRoom, activities);
    az.postTimeStep();
    z_Kitchen.setWindowState(1);
    az.step(z_Kitchen, z_Kitchen, activities);
    int second = az.getDesiredWindowDuration();
    az.postTimeStep();
    if (first > 0 && second > 0) {
      EXPECT_FALSE(first == second);
    }
  }

  for (int i = 0; i < 100000; i++) {
    int first = az.getDesiredWindowDuration();
    z_Kitchen.setWindowState(0);
    az.step(z_Kitchen, z_LivingRoom, activities);
    z_Kitchen.setWindowState(1);
    az.postTimeStep();
    az.step(z_LivingRoom, z_Kitchen, activities);
    int second = az.getDesiredWindowDuration();
    az.postTimeStep();
    if (first > 0 && second > 0) {
      //  EXPECT_FALSE(first == second);
    }
  }
}
