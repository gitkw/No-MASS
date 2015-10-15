
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
  while(!a.currentlyInZone(z_MasterBedroom)){
    a.step(&stateMachine);
    break;
  }


  ASSERT_NEAR(a.getCurrentRadientGains(), 69.5992,0.001);


}
