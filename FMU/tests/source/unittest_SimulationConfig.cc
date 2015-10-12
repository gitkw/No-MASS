

#include <limits.h>

#include "SimulationConfig.h"

#include "gtest/gtest.h"



TEST(SimulationConfig, SimulationConfig) {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration("tests/Files/SimulationConfig.xml");

  EXPECT_EQ(SimulationConfig::agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(1).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(0).profile.size(), 24);
  EXPECT_EQ(SimulationConfig::agents.at(1).profile.size(), 24);
  EXPECT_EQ(SimulationConfig::agents.at(0).profile.at(0), "0.908047477,0.02029764,0.017328265,0.001707529,0.002495997,0.000573,0.006271609,0.0010838340,0.001585154,0.04060991");


}

TEST(SimulationConfig, SimulationConfig1) {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration("tests/Files/SimulationConfig1.xml");

  EXPECT_EQ(SimulationConfig::agents.at(0).age,"age2");
  EXPECT_EQ(SimulationConfig::agents.at(0).computer,"computer0");
  EXPECT_EQ(SimulationConfig::agents.at(0).civstat,"civstat1");
  EXPECT_EQ(SimulationConfig::agents.at(0).unemp,"unemp0");
  EXPECT_EQ(SimulationConfig::agents.at(0).retired,"retired1");
  EXPECT_EQ(SimulationConfig::agents.at(0).edtry,"edtry1");
  EXPECT_EQ(SimulationConfig::agents.at(0).famstat,"famstat3");
  EXPECT_EQ(SimulationConfig::agents.at(0).sex,"sex2");
  EXPECT_EQ(SimulationConfig::agents.at(1).age,"age2");
  EXPECT_EQ(SimulationConfig::agents.at(1).computer,"computer0");
  EXPECT_EQ(SimulationConfig::agents.at(1).civstat,"civstat1");
  EXPECT_EQ(SimulationConfig::agents.at(1).unemp,"unemp0");
  EXPECT_EQ(SimulationConfig::agents.at(1).retired,"retired1");
  EXPECT_EQ(SimulationConfig::agents.at(1).edtry,"edtry1");
  EXPECT_EQ(SimulationConfig::agents.at(1).famstat,"famstat3");
  EXPECT_EQ(SimulationConfig::agents.at(1).sex,"sex2");
  EXPECT_EQ(SimulationConfig::agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(1).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::ActivityFile, "tests/Files/Activity.xml");



}
