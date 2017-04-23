// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <memory>

#include "tests/Gen.hpp"
#include "DataStore.hpp"

#include "Occupant.hpp"
#include "gtest/gtest.h"

class Test_Occupant : public ::testing::Test {
 protected:
    std::shared_ptr<Building_Zone> z_KitchenPtr;
    std::shared_ptr<Building_Zone> z_LivingRoomPtr;
    std::shared_ptr<Building_Zone> z_BathroomPtr;
    std::shared_ptr<Building_Zone> z_MasterBedroomPtr;
    std::shared_ptr<Building_Zone> z_OfficePtr;
    std::shared_ptr<Building_Zone> z_OfficePtr2;
    std::shared_ptr<Building_Zone> z_OutPtr;

    std::shared_ptr<Occupant> a;
    std::shared_ptr<Occupant> b;

    std::vector<std::shared_ptr<Building_Zone>> v;
    virtual void SetUp();
    virtual void SetUpSim2();
    virtual void SetUpSim4();
};

void Test_Occupant::SetUp() {
  Configuration::reset();
    v.clear();
}

void Test_Occupant::SetUpSim2() {
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = true;
  Configuration::info.shading = false;
  Configuration::info.lights = false;

  ConfigStructZone zs;
  zs.name = "out";
  zs.id = 0;
  zs.activities = {9};
  Building_Zone z_Out;
  z_Out.setName(zs.name);
  z_Out.setActive(true);
  z_Out.setup(zs);

  z_OutPtr = std::make_shared<Building_Zone>(z_Out);

  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");

  DataStore::addVariable("Block2:MasterBedroomZoneMeanAirTemperature");
  DataStore::addVariable("Block2:MasterBedroomZoneAirRelativeHumidity");
  DataStore::addVariable("Block2:MasterBedroomZoneMeanRadiantTemperature");

  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");

  DataStore::addVariable("Block1:LivingRoomZoneMeanAirTemperature");
  DataStore::addVariable("Block1:LivingRoomZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:LivingRoomZoneMeanRadiantTemperature");

  DataStore::addVariable("Block2:BathroomZoneMeanAirTemperature");
  DataStore::addVariable("Block2:BathroomZoneAirRelativeHumidity");
  DataStore::addVariable("Block2:BathroomZoneMeanRadiantTemperature");

  DataStore::addVariable("Block2:OfficeZoneMeanAirTemperature");
  DataStore::addVariable("Block2:OfficeZoneAirRelativeHumidity");
  DataStore::addVariable("Block2:OfficeZoneMeanRadiantTemperature");

  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);

  DataStore::addValueS("Block2:MasterBedroomZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block2:MasterBedroomZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block2:MasterBedroomZoneMeanRadiantTemperature", 21);

  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 21);

  DataStore::addValueS("Block1:LivingRoomZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:LivingRoomZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:LivingRoomZoneMeanRadiantTemperature", 21);

  DataStore::addValueS("Block2:BathroomZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block2:BathroomZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block2:BathroomZoneMeanRadiantTemperature", 21);

  DataStore::addValueS("Block2:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block2:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block2:OfficeZoneMeanRadiantTemperature", 21);

  zs.name = "Block1:Kitchen";
  zs.id = 1;
  zs.activities = {4, 5};
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setActive(true);
  z_Kitchen.setup(zs);
  z_KitchenPtr = std::make_shared<Building_Zone>(z_Kitchen);
  zs.name = "Block1:LivingRoom";
  zs.id = 2;
  zs.activities = {1, 7, 2};
  Building_Zone z_LivingRoom;
  z_LivingRoom.setName(zs.name);
  z_LivingRoom.setActive(true);
  z_LivingRoom.setup(zs);
  z_LivingRoomPtr = std::make_shared<Building_Zone>(z_LivingRoom);
  zs.name = "Block2:Bathroom";
  zs.id = 3;
  zs.activities = {6, 8};
  Building_Zone z_Bathroom;
  z_Bathroom.setName(zs.name);
  z_Bathroom.setActive(true);
  z_Bathroom.setup(zs);
  z_BathroomPtr = std::make_shared<Building_Zone>(z_Bathroom);
  zs.name = "Block2:MasterBedroom";
  zs.id = 4;
  zs.activities = {0};
  Building_Zone z_MasterBedroom;
  z_MasterBedroom.setName(zs.name);
  z_MasterBedroom.setActive(true);
  z_MasterBedroom.setup(zs);
  z_MasterBedroomPtr = std::make_shared<Building_Zone>(z_MasterBedroom);
  zs.name = "Block2:Office";
  zs.id = 5;
  zs.activities = {3};
  Building_Zone z_Office;
  z_Office.setName(zs.name);
  z_Office.setActive(true);
  z_Office.setup(zs);
  z_OfficePtr = std::make_shared<Building_Zone>(z_Office);

  v.push_back(z_MasterBedroomPtr);
  v.push_back(z_KitchenPtr);
  v.push_back(z_LivingRoomPtr);
  v.push_back(z_BathroomPtr);
  v.push_back(z_OfficePtr);
  v.push_back(z_OutPtr);


  ConfigStructAgent agentS = Configuration::buildings[0].agents[0];
  Occupant agent;
  agent.setBuildingID(0);
  agent.setup(0, agentS, v);
  agent.step();

  agentS = Configuration::buildings[0].agents[1];
  Occupant agentb;
  agentb.setBuildingID(0);
  agentb.setup(1, agentS, v);
  agentb.step();

  a = std::make_shared<Occupant>(agent);
  b = std::make_shared<Occupant>(agentb);
}

void Test_Occupant::SetUpSim4() {
  Configuration::parseConfiguration(testFiles + "/SimulationConfig4.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = true;
  Configuration::info.shading = false;
  Configuration::info.lights = false;

  ConfigStructZone zs;
  zs.name = "out";
  zs.id = 0;
  zs.activities = {9};
  Building_Zone z_Out;
  z_Out.setName(zs.name);
  z_Out.setActive(true);
  z_Out.setup(zs);

  z_OutPtr = std::make_shared<Building_Zone>(z_Out);

  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");

  DataStore::addVariable("Block1:OfficeZoneMeanAirTemperature");
  DataStore::addVariable("Block1:OfficeZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:OfficeZoneMeanRadiantTemperature");
  DataStore::addVariable("Block2:OfficeZoneMeanAirTemperature");
  DataStore::addVariable("Block2:OfficeZoneAirRelativeHumidity");
  DataStore::addVariable("Block2:OfficeZoneMeanRadiantTemperature");
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);

  DataStore::addValueS("Block1:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:OfficeZoneMeanRadiantTemperature", 21);
  DataStore::addValueS("Block2:OfficeZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block2:OfficeZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block2:OfficeZoneMeanRadiantTemperature", 21);


  zs.name = "Block1:Office";
  zs.activities = {3};
  zs.id = 1;
  Building_Zone z_Office;
  z_Office.setName(zs.name);
  z_Office.setActive(true);
  z_Office.setup(zs);
  z_OfficePtr = std::make_shared<Building_Zone>(z_Office);
  zs.name = "Block2:Office";
  zs.activities = {3};
  zs.id = 2;
  Building_Zone z_Office2;
  z_Office2.setName(zs.name);
  z_Office2.setActive(true);
  z_Office2.setup(zs);
  z_OfficePtr2 = std::make_shared<Building_Zone>(z_Office2);

  v.push_back(z_OfficePtr);
  v.push_back(z_OfficePtr2);
  v.push_back(z_OutPtr);

  ConfigStructAgent agentS = Configuration::buildings[0].agents[0];
  Occupant agent;
  agent.setBuildingID(0);
  agent.setup(0, agentS, v);
  agent.step();

  agentS = Configuration::buildings[0].agents[1];
  Occupant agentb;
  agentb.setBuildingID(0);
  agentb.setup(1, agentS, v);
  agentb.step();

  a = std::make_shared<Occupant>(agent);
  b = std::make_shared<Occupant>(agentb);
}

TEST_F(Test_Occupant, windows) {
  SetUpSim2();
  int x = 0;
  while (true) {
    Configuration::step();
    a->step();

    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() > 0) {
        if (a->currentlyInZone(*zone)) {
          EXPECT_TRUE(a->isActionWindow(*zone));
        }
        if (a->previouslyInZone(*zone)) {
          EXPECT_TRUE(a->isActionWindow(*zone));
        }
        if (!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)) {
          EXPECT_FALSE(a->isActionWindow(*zone));
        }
      }
    }
    a->postTimeStep();
    x = x + 1;
    if (x > 1000) break;
  }
}

TEST_F(Test_Occupant, windowsTwo) {
  SetUpSim2();
  int x = 0;
  while (true) {
    Configuration::step();
    a->step();
    b->step();
    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() > 0) {
        if (a->currentlyInZone(*zone)) {
          EXPECT_TRUE(a->isActionWindow(*zone));
        }
        if (a->previouslyInZone(*zone)) {
          EXPECT_TRUE(a->isActionWindow(*zone));
        }
        if (!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)) {
          EXPECT_FALSE(a->isActionWindow(*zone));
        }
        if (b->currentlyInZone(*zone)) {
          EXPECT_TRUE(b->isActionWindow(*zone));
        }
        if (b->previouslyInZone(*zone)) {
          EXPECT_TRUE(b->isActionWindow(*zone));
        }
        if (!b->currentlyInZone(*zone) && !b->previouslyInZone(*zone)) {
          EXPECT_FALSE(b->isActionWindow(*zone));
        }
      }
    }
    a->postTimeStep();
    b->postTimeStep();
    x = x +1;
    if (x > 1000) break;
  }
}

TEST_F(Test_Occupant, office) {
  SetUpSim4();

  int x = 0;
  while (true) {
    Configuration::step();
    a->step();
    b->step();
    for (std::shared_ptr<Building_Zone> &zone : v) {
      if (zone->getId() == 2) {
        EXPECT_FALSE(a->currentlyInZone(*zone));
      }
      if (zone->getId() == 1) {
        EXPECT_FALSE(b->currentlyInZone(*zone));
      }
    }
    a->postTimeStep();
    b->postTimeStep();
    x = x +1;
    if (x > 1000) break;
  }
}
