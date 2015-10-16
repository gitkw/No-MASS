
#include <limits.h>

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

TEST(Agent, error) {
  SimulationConfig::parseConfiguration("tests/Files/SimulationConfig2.xml");


  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  StateMachine stateMachine;
  State_Out out;
  stateMachine.addState(out);
  State_Present present;


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


  Zone z_Kitchen("Block1:Kitchen");
  Zone z_LivingRoom("Block1:LivingRoom");
  Zone z_Bathroom("Block2:Bathroom");
  Zone z_MasterBedroom("Block2:MasterBedroom");
  Zone z_Office("Block2:Office");

  State_Sleep sleep;
  sleep.setZonePtr(&(z_MasterBedroom));
  State_Passive passive;
  passive.setZonePtr(&(z_LivingRoom));
  State_Washing_Appliance washingAppliance;
  washingAppliance.setZonePtr(&(z_Bathroom));
  State_Washing washing;
  washing.setZonePtr(&(z_Bathroom));
  State_Audio_Visual audioVisual;
  audioVisual.setZonePtr(&(z_LivingRoom));
  State_Cleaning cleaning;
  cleaning.setZonePtr(&(z_Kitchen));
  State_Cooking cooking;
  cooking.setZonePtr(&(z_Kitchen));
  State_Metabolic metabolic;
  metabolic.setZonePtr(&(z_LivingRoom));
  State_IT it;
  it.setZonePtr(&(z_Office));
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



  Agent a(0);

  a.step(&stateMachine);
/*
  while(!a.currentlyInZone(z_MasterBedroom)){
    SimulationConfig::step();
    a.step(&stateMachine);
  }
  ASSERT_NEAR(a.getCurrentRadientGains(z_MasterBedroom), 46.9452,0.001);

  while(!a.currentlyInZone(z_Kitchen)){
    SimulationConfig::step();
    a.step(&stateMachine);
  }
  ASSERT_NEAR(a.getCurrentRadientGains(z_Kitchen), 99.4269,0.001);

  while(!a.currentlyInZone(z_Bathroom)){
    SimulationConfig::step();
    a.step(&stateMachine);
  }
  ASSERT_NEAR(a.getCurrentRadientGains(z_Bathroom), 99.426920,0.001);

  while(!a.currentlyInZone(z_Office)){
    SimulationConfig::step();
    a.step(&stateMachine);
  }
  ASSERT_NEAR(a.getCurrentRadientGains(z_Office), 99.426920,0.001);

  while(!a.currentlyInZone(z_LivingRoom)){
    SimulationConfig::step();
    a.step(&stateMachine);
  }
  ASSERT_NEAR(a.getCurrentRadientGains(z_LivingRoom), 70.069288,0.001);

*/

}
