// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <memory>

#include "Gen.h"
#include "DataStore.h"

#include "Agent.h"
#include "gtest/gtest.h"

class Test_Agent : public ::testing::Test {
 protected:
    std::shared_ptr<Building_Zone> z_KitchenPtr;
    std::shared_ptr<Building_Zone> z_LivingRoomPtr;
    std::shared_ptr<Building_Zone> z_BathroomPtr;
    std::shared_ptr<Building_Zone> z_MasterBedroomPtr;
    std::shared_ptr<Building_Zone> z_OfficePtr;
    std::shared_ptr<Building_Zone> z_OfficePtr2;
    std::shared_ptr<Building_Zone> z_OutPtr;

    std::shared_ptr<Agent> a;
    std::shared_ptr<Agent> b;

    std::vector<std::shared_ptr<Building_Zone>> v;
    virtual void SetUp();
    virtual void SetUpSim2();
    virtual void SetUpSim4();
};

void Test_Agent::SetUp() {
  SimulationConfig::reset();
  v.clear();
}

void Test_Agent::SetUpSim2() {
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = true;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  ZoneStruct zs;
  zs.name = "Out";
  zs.id = 0;
  zs.activities = {9};
  Building_Zone z_Out;
  z_Out.setName(zs.name);
  z_Out.setup(zs);

  z_OutPtr = std::make_shared<Building_Zone>(z_Out);

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);

  DataStore::addValue("Block2:MasterBedroomZoneMeanAirTemperature", 21);
  DataStore::addValue("Block2:MasterBedroomZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block2:MasterBedroomZoneMeanRadiantTemperature", 21);

  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 21);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 21);

  DataStore::addValue("Block1:LivingRoomZoneMeanAirTemperature", 21);
  DataStore::addValue("Block1:LivingRoomZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block1:LivingRoomZoneMeanRadiantTemperature", 21);

  DataStore::addValue("Block2:BathroomZoneMeanAirTemperature", 21);
  DataStore::addValue("Block2:BathroomZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block2:BathroomZoneMeanRadiantTemperature", 21);

  DataStore::addValue("Block2:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValue("Block2:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block2:OfficeZoneMeanRadiantTemperature", 21);

  zs.name = "Block1:Kitchen";
  zs.id = 1;
  zs.activities = {4, 5};
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  z_KitchenPtr = std::make_shared<Building_Zone>(z_Kitchen);
  zs.name = "Block1:LivingRoom";
  zs.id = 2;
  zs.activities = {1, 7, 2};
  Building_Zone z_LivingRoom;
  z_LivingRoom.setName(zs.name);
  z_LivingRoom.setup(zs);
  z_LivingRoomPtr = std::make_shared<Building_Zone>(z_LivingRoom);
  zs.name = "Block2:Bathroom";
  zs.id = 3;
  zs.activities = {6, 8};
  Building_Zone z_Bathroom;
  z_Bathroom.setName(zs.name);
  z_Bathroom.setup(zs);
  z_BathroomPtr = std::make_shared<Building_Zone>(z_Bathroom);
  zs.name = "Block2:MasterBedroom";
  zs.id = 4;
  zs.activities = {0};
  Building_Zone z_MasterBedroom;
  z_MasterBedroom.setName(zs.name);
  z_MasterBedroom.setup(zs);
  z_MasterBedroomPtr = std::make_shared<Building_Zone>(z_MasterBedroom);
  zs.name = "Block2:Office";
  zs.id = 5;
  zs.activities = {3};
  Building_Zone z_Office;
  z_Office.setName(zs.name);
  z_Office.setup(zs);
  z_OfficePtr = std::make_shared<Building_Zone>(z_Office);

  v.push_back(z_MasterBedroomPtr);
  v.push_back(z_KitchenPtr);
  v.push_back(z_LivingRoomPtr);
  v.push_back(z_BathroomPtr);
  v.push_back(z_OfficePtr);
  v.push_back(z_OutPtr);


  Agent agent;
  agent.setup(0, v);
  agent.step();
  Agent agentb;
  agentb.setup(1, v);
  agentb.step();

  a = std::make_shared<Agent>(agent);
  b = std::make_shared<Agent>(agentb);
}

void Test_Agent::SetUpSim4() {
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig4.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = true;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  ZoneStruct zs;
  zs.name = "Out";
  zs.id = 0;
  zs.activities = {9};
  Building_Zone z_Out;
  z_Out.setName(zs.name);
  z_Out.setup(zs);

  z_OutPtr = std::make_shared<Building_Zone>(z_Out);

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);

  DataStore::addValue("Block1:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValue("Block1:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block1:OfficeZoneMeanRadiantTemperature", 21);
  DataStore::addValue("Block2:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValue("Block2:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block2:OfficeZoneMeanRadiantTemperature", 21);


  zs.name = "Block1:Office";
  zs.activities = {3};
  zs.id = 1;
  Building_Zone z_Office;
  z_Office.setName(zs.name);
  z_Office.setup(zs);
  z_OfficePtr = std::make_shared<Building_Zone>(z_Office);
  zs.name = "Block2:Office";
  zs.activities = {3};
  zs.id = 2;
  Building_Zone z_Office2;
  z_Office2.setName(zs.name);
  z_Office2.setup(zs);
  z_OfficePtr2 = std::make_shared<Building_Zone>(z_Office2);

  v.push_back(z_OfficePtr);
  v.push_back(z_OfficePtr2);
  v.push_back(z_OutPtr);

  Agent agent;
  agent.setup(0, v);
  agent.step();
  Agent agentb;
  agentb.setup(1, v);
  agentb.step();

  a = std::make_shared<Agent>(agent);
  b = std::make_shared<Agent>(agentb);
}

TEST_F(Test_Agent, windows) {
  SetUpSim2();
  int x = 0;
  while (true) {
    SimulationConfig::step();
    a->step();

    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() > 0) {
        if (a->currentlyInZone(*zone)) {
          ASSERT_TRUE(a->isActionWindow(*zone));
        }
        if (a->previouslyInZone(*zone)) {
          ASSERT_TRUE(a->isActionWindow(*zone));
        }
        if (!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)) {
          ASSERT_FALSE(a->isActionWindow(*zone));
        }
      }
    }
    a->postTimeStep();
    x = x + 1;
    if (x > 1000) break;
  }
}

TEST_F(Test_Agent, windowsTwo) {
  SetUpSim2();
  int x = 0;
  while (true) {
    SimulationConfig::step();
    a->step();
    b->step();
    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() > 0) {
        if (a->currentlyInZone(*zone)) {
          ASSERT_TRUE(a->isActionWindow(*zone));
        }
        if (a->previouslyInZone(*zone)) {
          ASSERT_TRUE(a->isActionWindow(*zone));
        }
        if (!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)) {
          ASSERT_FALSE(a->isActionWindow(*zone));
        }
        if (b->currentlyInZone(*zone)) {
          ASSERT_TRUE(b->isActionWindow(*zone));
        }
        if (b->previouslyInZone(*zone)) {
          ASSERT_TRUE(b->isActionWindow(*zone));
        }
        if (!b->currentlyInZone(*zone) && !b->previouslyInZone(*zone)) {
          ASSERT_FALSE(b->isActionWindow(*zone));
        }
      }
    }
    a->postTimeStep();
    b->postTimeStep();
    x = x +1;
    if (x > 1000) break;
  }
}

TEST_F(Test_Agent, office) {
  SetUpSim4();

  int x = 0;
  while (true) {
    SimulationConfig::step();
    a->step();
    b->step();
    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() == 2) {
        ASSERT_FALSE(a->currentlyInZone(*zone));
      }
      if (zone->getId() == 1) {
        ASSERT_FALSE(b->currentlyInZone(*zone));
      }
    }
    a->postTimeStep();
    b->postTimeStep();
    x = x +1;
    if (x > 1000) break;
  }
}
