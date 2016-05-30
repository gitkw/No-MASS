// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <memory>

#include "Gen.h"
#include "StateMachine.h"
#include "State_Out.h"
#include "State_Present.h"
#include "State_Sleep.h"
#include "State_Passive.h"
#include "State_IT.h"
#include "State_Washing_Appliance.h"
#include "State_Audio_Visual.h"
#include "State_Cleaning.h"
#include "State_Cooking.h"
#include "State_Washing.h"
#include "State_Metabolic.h"
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
   std::shared_ptr<Building_Zone> z_OutPtr;


    std::shared_ptr<Agent> a;
    std::shared_ptr<Agent> b;
    StateMachine stateMachine;
    std::vector<std::shared_ptr<Building_Zone>> v;
    virtual void SetUp();
};

void Test_Agent::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = true;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  State_Out out;

  ZoneStruct zs;
  zs.name = "Out";
  zs.id = 0;
  Building_Zone z_Out(zs);

  out.setZonePtr(std::make_shared<Building_Zone>(z_Out));
  stateMachine.addState(out);
  State_Present present;
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
  Building_Zone z_Kitchen(zs);
  z_KitchenPtr = std::make_shared<Building_Zone>(z_Kitchen);
  zs.name = "Block1:LivingRoom";
  zs.id = 2;
  Building_Zone z_LivingRoom(zs);
  z_LivingRoomPtr = std::make_shared<Building_Zone>(z_LivingRoom);
  zs.name = "Block2:Bathroom";
  zs.id = 3;
  Building_Zone z_Bathroom(zs);
  z_BathroomPtr = std::make_shared<Building_Zone>(z_Bathroom);
  zs.name = "Block2:MasterBedroom";
  zs.id = 4;
  Building_Zone z_MasterBedroom(zs);
  z_MasterBedroomPtr = std::make_shared<Building_Zone>(z_MasterBedroom);
  zs.name = "Block2:Office";
  zs.id = 5;
  Building_Zone z_Office(zs);
  z_OfficePtr = std::make_shared<Building_Zone>(z_Office);

  State_Sleep sleep;
  sleep.setZonePtr(std::make_shared<Building_Zone>(z_MasterBedroom));
  State_Passive passive;
  passive.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
  State_Washing_Appliance washingAppliance;
  washingAppliance.setZonePtr(std::make_shared<Building_Zone>(z_Bathroom));
  State_Washing washing;
  washing.setZonePtr(std::make_shared<Building_Zone>(z_Bathroom));
  State_Audio_Visual audioVisual;
  audioVisual.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
  State_Cleaning cleaning;
  cleaning.setZonePtr(std::make_shared<Building_Zone>(z_Kitchen));
  State_Cooking cooking;
  cooking.setZonePtr(std::make_shared<Building_Zone>(z_Kitchen));
  State_Metabolic metabolic;
  metabolic.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
  State_IT it;
  it.setZonePtr(std::make_shared<Building_Zone>(z_Office));
  present.addState(sleep);
  present.addState(passive);
  present.addState(washingAppliance);
  present.addState(washing);
  present.addState(audioVisual);
  present.addState(cleaning);
  present.addState(cooking);
  present.addState(metabolic);
  present.addState(it);

  stateMachine.addState(present);


  v.push_back(z_MasterBedroomPtr);
  v.push_back(z_KitchenPtr);
  v.push_back(z_LivingRoomPtr);
  v.push_back(z_BathroomPtr);
  v.push_back(z_OfficePtr);


  Agent agent;
  agent.setup(0, v);
  agent.setState(out);
  agent.step(&stateMachine);
  Agent agentb;
  agentb.setup(1, v);
  agentb.setState(out);
  agentb.step(&stateMachine);

  a = std::make_shared<Agent>(agent);
  b = std::make_shared<Agent>(agentb);

}

TEST_F(Test_Agent, windows) {
  int x = 0;
  while (true) {
    SimulationConfig::step();
    a->step(&stateMachine);

    for (std::shared_ptr<Building_Zone> &zone : v) {
      if(a->currentlyInZone(*zone)){
        ASSERT_TRUE(a->isActionWindow(*zone));
      }
      if(a->previouslyInZone(*zone)){
        ASSERT_TRUE(a->isActionWindow(*zone));
      }
      if(!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)){
        ASSERT_FALSE(a->isActionWindow(*zone));
      }
    }
    a->postTimeStep();
    x = x + 1;
    if (x > 10000) break;
  }
}

TEST_F(Test_Agent, windowsTwo) {

  int x = 0;
  while (true) {
    SimulationConfig::step();
    a->step(&stateMachine);
    b->step(&stateMachine);
    for (std::shared_ptr<Building_Zone> &zone : v) {
      if(a->currentlyInZone(*zone)){
        ASSERT_TRUE(a->isActionWindow(*zone));
      }
      if(a->previouslyInZone(*zone)){
        ASSERT_TRUE(a->isActionWindow(*zone));
      }
      if(!a->currentlyInZone(*zone) && !a->previouslyInZone(*zone)){
        ASSERT_FALSE(a->isActionWindow(*zone));
      }
      if(b->currentlyInZone(*zone)){
        ASSERT_TRUE(b->isActionWindow(*zone));
      }
      if(b->previouslyInZone(*zone)){
        ASSERT_TRUE(b->isActionWindow(*zone));
      }
      if(!b->currentlyInZone(*zone) && !b->previouslyInZone(*zone)){
        ASSERT_FALSE(b->isActionWindow(*zone));
      }
    }
    a->postTimeStep();
    b->postTimeStep();
    x = x +1;
    if (x > 10000) break;
  }
}
