// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "Gen.h"
#include "Occupant_Zone.h"
#include "DataStore.h"
#include "gtest/gtest.h"

class Test_Occupant_Zone : public ::testing::Test {
 protected:
    std::shared_ptr<Building_Zone> z_KitchenPtr;
    std::shared_ptr<Building_Zone> z_LivingRoomPtr;
    std::shared_ptr<Occupant_Zone> az;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Zone::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = true;
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

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  z_KitchenPtr = std::make_shared<Building_Zone>(z_Kitchen);
  zs.name = "Block1:LivingRoom";
  zs.id = 2;
  Building_Zone z_LivingRoom;
  z_LivingRoom.setName(zs.name);
  z_LivingRoom.setup(zs);
  z_LivingRoomPtr = std::make_shared<Building_Zone>(z_LivingRoom);

  agentStruct agent = SimulationConfig::buildings[0].agents.at(1);
  Occupant_Zone aZone;
  aZone.setup(z_Kitchen, 1, agent);

  az = std::make_shared<Occupant_Zone>(aZone);
}


TEST_F(Test_Occupant_Zone, oneZone) {
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 23);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 26);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 50);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 23);

  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  SimulationConfig::step();
  az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
  EXPECT_FALSE(az->getDesiredWindowState());
  az->postTimeStep();

  while (!az->getDesiredWindowState() ||
          az->getDesiredWindowDuration() < 1000) {
      SimulationConfig::step();
      activities.push_back(6);
      az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
      z_KitchenPtr->setWindowState(az->getDesiredWindowState());
      az->postTimeStep();
  }
  int previousDuration = az->getDesiredWindowDuration();
  EXPECT_GT(previousDuration, 995);
  while (az->getDesiredWindowDuration() > 0) {
    SimulationConfig::step();
    activities.push_back(6);
    az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
    z_KitchenPtr->setWindowState(az->getDesiredWindowState());
    previousDuration = previousDuration - 5;
    if (previousDuration < 0) previousDuration = 0;
    if (previousDuration > 0) {
      EXPECT_TRUE(az->getDesiredWindowState());
      EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration);
    } else {
      EXPECT_FALSE(az->getDesiredWindowState());
      EXPECT_EQ(az->getDesiredWindowDuration(), 0);
    }
    az->postTimeStep();
  }
}


TEST_F(Test_Occupant_Zone, twoZone) {
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 23);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 26);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 50);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 23);

  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  activities.push_back(6);
  SimulationConfig::step();
  az->step(*z_KitchenPtr, *z_LivingRoomPtr, activities);

  EXPECT_FALSE(az->getDesiredWindowState());
  az->postTimeStep();

  while (!az->getDesiredWindowState() ||
          az->getDesiredWindowDuration() < 1000) {
      SimulationConfig::step();
      activities.push_back(6);
      az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
      z_KitchenPtr->setWindowState(az->getDesiredWindowState());
      az->postTimeStep();
  }
  int previousDuration = az->getDesiredWindowDuration();

  EXPECT_GT(previousDuration, 995);

  SimulationConfig::step();
  activities.push_back(6);
  az->step(*z_LivingRoomPtr, *z_KitchenPtr, activities);
  z_KitchenPtr->setWindowState(az->getDesiredWindowState());
  EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration);
  EXPECT_GT(az->getDesiredWindowDuration() , 995);
  EXPECT_TRUE(az->getDesiredWindowState());
  az->postTimeStep();

  for (int i = 0; i < 10000; i++) {
    az->step(*z_LivingRoomPtr, *z_LivingRoomPtr, activities);
    EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration);
    EXPECT_GT(az->getDesiredWindowDuration() , 995);
    EXPECT_TRUE(az->getDesiredWindowState());
    az->postTimeStep();
  }
  az->step(*z_KitchenPtr, *z_LivingRoomPtr, activities);
  z_KitchenPtr->setWindowState(az->getDesiredWindowState());
  previousDuration = az->getDesiredWindowDuration();
  EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration);
  EXPECT_TRUE(az->getDesiredWindowState());
  az->postTimeStep();

  az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
  z_KitchenPtr->setWindowState(az->getDesiredWindowState());
  EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration - 5);
  EXPECT_TRUE(az->getDesiredWindowState());
  az->postTimeStep();

  z_KitchenPtr->setWindowState(0);
  az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
  EXPECT_EQ(az->getDesiredWindowDuration(), 0);
  EXPECT_FALSE(az->getDesiredWindowState());
  az->postTimeStep();

  while (!az->getDesiredWindowState() ||
          az->getDesiredWindowDuration() < 1000) {
    z_KitchenPtr->setWindowState(0);
    az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
    az->postTimeStep();
  }
  previousDuration = az->getDesiredWindowDuration();
  EXPECT_EQ(az->getDesiredWindowDuration(), previousDuration);
  EXPECT_TRUE(az->getDesiredWindowState());
  az->postTimeStep();
  z_KitchenPtr->setWindowState(0);
  az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
  EXPECT_FALSE(az->getDesiredWindowState());
  az->postTimeStep();
  z_KitchenPtr->setWindowState(1);
  az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
  EXPECT_FALSE(az->getDesiredWindowState());
  az->postTimeStep();

  for (int i = 0; i < 100000; i++) {
    int first = az->getDesiredWindowDuration();
    z_KitchenPtr->setWindowState(0);
    az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
    az->postTimeStep();
    z_KitchenPtr->setWindowState(1);
    az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
    int second = az->getDesiredWindowDuration();
    az->postTimeStep();
    if (first > 0 && second > 0) {
      EXPECT_FALSE(first == second);
    }
  }
  for (int i = 0; i < 100000; i++) {
    int first = az->getDesiredWindowDuration();
    z_KitchenPtr->setWindowState(0);
    az->step(*z_KitchenPtr, *z_LivingRoomPtr, activities);
    az->postTimeStep();
    z_KitchenPtr->setWindowState(1);
    az->step(*z_KitchenPtr, *z_KitchenPtr, activities);
    int second = az->getDesiredWindowDuration();
    az->postTimeStep();
    if (first > 0 && second > 0) {
      EXPECT_FALSE(first == second);
    }
  }

  for (int i = 0; i < 100000; i++) {
    int first = az->getDesiredWindowDuration();
    z_KitchenPtr->setWindowState(0);
    az->step(*z_KitchenPtr, *z_LivingRoomPtr, activities);
    z_KitchenPtr->setWindowState(1);
    az->postTimeStep();
    az->step(*z_LivingRoomPtr, *z_KitchenPtr, activities);
    int second = az->getDesiredWindowDuration();
    az->postTimeStep();
    if (first > 0 && second > 0) {
      //  EXPECT_FALSE(first == second);
    }
  }
}
