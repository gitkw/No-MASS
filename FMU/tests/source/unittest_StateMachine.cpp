// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "StateMachine.hpp"
#include "State.hpp"
#include "Building_Zone.hpp"
#include "Configuration.hpp"

#include "gtest/gtest.h"

TEST(StateMachine, statesPage) {
  ConfigStructZone zs;
  zs.name = "Block1:Zone1";
  zs.id = 1;
  Building_Zone z;
  z.setName(zs.name);
  z.setup(zs);

  ConfigStructZone zs_out;
  zs_out.name = "out";
  zs_out.id = 0;
  Building_Zone z_out;
  z_out.setName(zs_out.name);
  z_out.setup(zs_out);

  StateMachine stateMachine;
  State out(9, 0, 1, "Out");
  out.setZonePtr(std::make_shared<Building_Zone>(z_out));
  stateMachine.addState(out);

  EXPECT_EQ(stateMachine.numberOfStates(),  1);
  EXPECT_EQ(out.numberOfSubStates(),  0);

  State present(-100,-1,-1,"");
  State it(3, 70, 1, "IT");

  it.setZonePtr(std::make_shared<Building_Zone>(z));
  present.addState(it);

  stateMachine.addState(present);

  EXPECT_EQ(stateMachine.numberOfStates(), 2);
  EXPECT_EQ(out.numberOfSubStates(), 0);
  EXPECT_EQ(it.numberOfSubStates(), 0);
  EXPECT_EQ(present.numberOfSubStates(), 1);

  State s = stateMachine.transistionTo(9);
  EXPECT_EQ(s.getId(), 9);
  EXPECT_EQ(s.getMetabolicRate(), 0);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Out"));


  s = stateMachine.transistionTo(3);
  EXPECT_EQ(s.getId(), 3);
  EXPECT_EQ(s.getMetabolicRate(), 70);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("IT"));
//  EXPECT_EQ(s.getZonePtr()->getName(), "Block1:Zone1");
}

TEST(StateMachine, statesActivity) {
  ConfigStructZone zs_out;
  zs_out.name = "out";
  zs_out.id = 1;
  Building_Zone z_out;
  z_out.setName(zs_out.name);
  z_out.setup(zs_out);

  StateMachine stateMachine;
  State out(9, 0, 1, "Out");
  out.setZonePtr(std::make_shared<Building_Zone>(z_out));
  stateMachine.addState(out);
  State present(-100,-1,-1,"");
  EXPECT_EQ(stateMachine.numberOfStates(), 1);
  EXPECT_EQ(out.numberOfSubStates(), 0);
  ConfigStructZone zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  zs.name = "Block1:LivingRoom";
  zs.id = 1;
  Building_Zone z_LivingRoom;
  z_LivingRoom.setName(zs.name);
  z_LivingRoom.setup(zs);
  zs.name = "Block2:Bathroom";
  zs.id = 1;
  Building_Zone z_Bathroom;
  z_Bathroom.setName(zs.name);
  z_Bathroom.setup(zs);
  zs.name = "Block2:MasterBedroom";
  zs.id = 1;
  Building_Zone z_MasterBedroom;
  z_MasterBedroom.setName(zs.name);
  z_MasterBedroom.setup(zs);
  zs.name = "Block2:Office";
  zs.id = 1;
  Building_Zone z_Office;
  z_Office.setName(zs.name);
  z_Office.setup(zs);

  State sleep(0, 46, 2.55, "Sleep");
  State passive(1, 58, 1, "Passive");
  State audioVisual(2, 70, 1, "AudioVisual");
  State it(3, 70, 1, "IT");
  State cooking(4, 116, 1, "Cooking");
  State cleaning(5, 116, 1, "Cleaning");
  State washing(6, 116, 0, "Washing");
  State metabolic(7, 93, 1, "Metabolic");
  State washingAppliance(8, 116, 1, "WashingAppliance");

  sleep.setZonePtr(std::make_shared<Building_Zone>(z_MasterBedroom));
  passive.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
  washingAppliance.setZonePtr(std::make_shared<Building_Zone>(z_Bathroom));
  washing.setZonePtr(std::make_shared<Building_Zone>(z_Bathroom));
  audioVisual.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
  cleaning.setZonePtr(std::make_shared<Building_Zone>(z_Kitchen));
  cooking.setZonePtr(std::make_shared<Building_Zone>(z_Kitchen));
  metabolic.setZonePtr(std::make_shared<Building_Zone>(z_LivingRoom));
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

  EXPECT_EQ(stateMachine.numberOfStates(), 2);
  EXPECT_EQ(out.numberOfSubStates(), 0);
  EXPECT_EQ(sleep.numberOfSubStates(), 0);
  EXPECT_EQ(passive.numberOfSubStates(), 0);
  EXPECT_EQ(washingAppliance.numberOfSubStates(), 0);
  EXPECT_EQ(washing.numberOfSubStates(), 0);
  EXPECT_EQ(audioVisual.numberOfSubStates(), 0);
  EXPECT_EQ(cleaning.numberOfSubStates(), 0);
  EXPECT_EQ(cooking.numberOfSubStates(), 0);
  EXPECT_EQ(metabolic.numberOfSubStates(), 0);
  EXPECT_EQ(it.numberOfSubStates(), 0);
  EXPECT_EQ(present.numberOfSubStates(), 9);

  State s = stateMachine.transistionTo(9);
  EXPECT_EQ(s.getId(), 9);
  EXPECT_EQ(s.getMetabolicRate(), 0);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Out"));
  s = stateMachine.transistionTo(8);
  EXPECT_EQ(s.getId(), 8);
  EXPECT_EQ(s.getMetabolicRate(), 116);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("WashingAppliance"));
  s = stateMachine.transistionTo(7);
  EXPECT_EQ(s.getId(), 7);
  EXPECT_EQ(s.getMetabolicRate(), 93);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Metabolic"));
  s = stateMachine.transistionTo(6);
  EXPECT_EQ(s.getId(), 6);
  EXPECT_EQ(s.getMetabolicRate(), 116);
  EXPECT_EQ(s.getClo(), 0);
  EXPECT_TRUE(s.isInActivity("Washing"));
  s = stateMachine.transistionTo(5);
  EXPECT_EQ(s.getId(), 5);
  EXPECT_EQ(s.getMetabolicRate(), 116);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Cleaning"));
  s = stateMachine.transistionTo(4);
  EXPECT_EQ(s.getId(), 4);
  EXPECT_EQ(s.getMetabolicRate(), 116);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Cooking"));
  s = stateMachine.transistionTo(3);
  EXPECT_EQ(s.getId(), 3);
  EXPECT_EQ(s.getMetabolicRate(), 70);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("IT"));
  s = stateMachine.transistionTo(2);
  EXPECT_EQ(s.getId(), 2);
  EXPECT_EQ(s.getMetabolicRate(), 70);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("AudioVisual"));
  s = stateMachine.transistionTo(1);
  EXPECT_EQ(s.getId(), 1);
  EXPECT_EQ(s.getMetabolicRate(), 58);
  EXPECT_EQ(s.getClo(), 1);
  EXPECT_TRUE(s.isInActivity("Passive"));
  s = stateMachine.transistionTo(0);
  EXPECT_EQ(s.getId(), 0);
  EXPECT_EQ(s.getMetabolicRate(), 46);
  EXPECT_EQ(s.getClo(), 2.55);
  EXPECT_TRUE(s.isInActivity("Sleep"));
}
