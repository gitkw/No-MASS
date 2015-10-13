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
#include "Zone.h"
#include "SimulationConfig.h"

#include "gtest/gtest.h"

TEST(StateMachine, statesPage) {

  SimulationConfig::parseConfiguration("tests/Files/SimulationConfig.xml");
  Zone z("Block1:Zone1");
  StateMachine stateMachine;
  State_Out out;
  stateMachine.addState(out);

  EXPECT_EQ(stateMachine.numberOfStates(),1);
  EXPECT_EQ(out.numberOfSubStates(),0);

  State_Present present;
  State_IT it;

  it.setZonePtr(&(z));
  present.addState(it);

  stateMachine.addState(present);

  EXPECT_EQ(stateMachine.numberOfStates(),2);
  EXPECT_EQ(out.numberOfSubStates(),0);
  EXPECT_EQ(it.numberOfSubStates(),0);
  EXPECT_EQ(present.numberOfSubStates(),1);

  State s = stateMachine.transistionTo(9);
  EXPECT_EQ(s.getId(),9);
  EXPECT_EQ(s.getMetabolicRate(),0);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Out");
  EXPECT_EQ(s.getActivity(),"Out");


  s = stateMachine.transistionTo(3);
  EXPECT_EQ(s.getId(),3);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:Zone1");
  EXPECT_EQ(s.getActivity(),"IT");
//  EXPECT_EQ(s.getZonePtr()->getName(),"Block1:Zone1");

}

TEST(StateMachine, statesActivity) {

  SimulationConfig::parseConfiguration("tests/Files/SimulationConfig1.xml");
  StateMachine stateMachine;
  State_Out out;
  stateMachine.addState(out);
  State_Present present;


  EXPECT_EQ(stateMachine.numberOfStates(),1);
  EXPECT_EQ(out.numberOfSubStates(),0);

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

  EXPECT_EQ(stateMachine.numberOfStates(),2);
  EXPECT_EQ(out.numberOfSubStates(),0);
  EXPECT_EQ(sleep.numberOfSubStates(),0);
  EXPECT_EQ(passive.numberOfSubStates(),0);
  EXPECT_EQ(washingAppliance.numberOfSubStates(),0);
  EXPECT_EQ(washing.numberOfSubStates(),0);
  EXPECT_EQ(audioVisual.numberOfSubStates(),0);
  EXPECT_EQ(cleaning.numberOfSubStates(),0);
  EXPECT_EQ(cooking.numberOfSubStates(),0);
  EXPECT_EQ(metabolic.numberOfSubStates(),0);
  EXPECT_EQ(it.numberOfSubStates(),0);
  EXPECT_EQ(present.numberOfSubStates(),9);


  State s = stateMachine.transistionTo(9);
  EXPECT_EQ(s.getId(),9);
  EXPECT_EQ(s.getMetabolicRate(),0);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Out");
  EXPECT_EQ(s.getActivity(),"Out");
  s = stateMachine.transistionTo(8);
  EXPECT_EQ(s.getId(),8);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block2:Bathroom");
  EXPECT_EQ(s.getActivity(),"WashingAppliance");
  s = stateMachine.transistionTo(7);
  EXPECT_EQ(s.getId(),7);
  EXPECT_EQ(s.getMetabolicRate(),93);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:LivingRoom");
  EXPECT_EQ(s.getActivity(),"Metabolic");
  s = stateMachine.transistionTo(6);
  EXPECT_EQ(s.getId(),6);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),0);
  EXPECT_EQ(s.getLocation(),"Block2:Bathroom");
  EXPECT_EQ(s.getActivity(),"Washing");
  s = stateMachine.transistionTo(5);
  EXPECT_EQ(s.getId(),5);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:Kitchen");
  EXPECT_EQ(s.getActivity(),"Cleaning");
  s = stateMachine.transistionTo(4);
  EXPECT_EQ(s.getId(),4);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:Kitchen");
  EXPECT_EQ(s.getActivity(),"Cooking");
  s = stateMachine.transistionTo(3);
  EXPECT_EQ(s.getId(),3);
  EXPECT_EQ(s.getMetabolicRate(),116);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block2:Office");
  EXPECT_EQ(s.getActivity(),"IT");
  s = stateMachine.transistionTo(2);
  EXPECT_EQ(s.getId(),2);
  EXPECT_EQ(s.getMetabolicRate(),70);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:LivingRoom");
  EXPECT_EQ(s.getActivity(),"AudioVisual");
  s = stateMachine.transistionTo(1);
  EXPECT_EQ(s.getId(),1);
  EXPECT_EQ(s.getMetabolicRate(),58);
  EXPECT_EQ(s.getClo(),1);
  EXPECT_EQ(s.getLocation(),"Block1:LivingRoom");
  EXPECT_EQ(s.getActivity(),"Passive");
  s = stateMachine.transistionTo(0);
  EXPECT_EQ(s.getId(),0);
  EXPECT_EQ(s.getMetabolicRate(),46);
  EXPECT_EQ(s.getClo(),2.55);
  EXPECT_EQ(s.getLocation(),"Block2:MasterBedroom");
  EXPECT_EQ(s.getActivity(),"Sleep");


}
