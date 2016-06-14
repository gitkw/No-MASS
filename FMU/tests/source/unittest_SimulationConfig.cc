// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "Gen.h"
#include "SimulationConfig.h"
#include "gtest/gtest.h"

TEST(SimulationConfig, SimulationConfig) {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");
  EXPECT_EQ(SimulationConfig::agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(1).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(0).profile.size(), 24);
  EXPECT_EQ(SimulationConfig::agents.at(1).profile.size(), 24);
  EXPECT_EQ(SimulationConfig::agents.at(0).profile.at(0), "0.919825741,0.013844725,0.011501807,0.001339251,0.002836063,0.000551457,0.00416508,0.002205826,0.001921411,0.041808638");
}

TEST(SimulationConfig, SimulationConfig1) {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration("SimulationConfig1.xml");
  EXPECT_EQ(SimulationConfig::agents.at(0).age, "age2");
  EXPECT_EQ(SimulationConfig::agents.at(0).computer, "computer0");
  EXPECT_EQ(SimulationConfig::agents.at(0).civstat, "civstat1");
  EXPECT_EQ(SimulationConfig::agents.at(0).unemp, "unemp0");
  EXPECT_EQ(SimulationConfig::agents.at(0).retired, "retired1");
  EXPECT_EQ(SimulationConfig::agents.at(0).edtry, "edtry1");
  EXPECT_EQ(SimulationConfig::agents.at(0).famstat, "famstat3");
  EXPECT_EQ(SimulationConfig::agents.at(0).sex, "sex2");
  EXPECT_EQ(SimulationConfig::agents.at(1).age, "age2");
  EXPECT_EQ(SimulationConfig::agents.at(1).computer, "computer0");
  EXPECT_EQ(SimulationConfig::agents.at(1).civstat, "civstat1");
  EXPECT_EQ(SimulationConfig::agents.at(1).unemp, "unemp0");
  EXPECT_EQ(SimulationConfig::agents.at(1).retired, "retired1");
  EXPECT_EQ(SimulationConfig::agents.at(1).edtry, "edtry1");
  EXPECT_EQ(SimulationConfig::agents.at(1).famstat, "famstat3");
  EXPECT_EQ(SimulationConfig::agents.at(1).sex, "sex2");
  EXPECT_EQ(SimulationConfig::agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(SimulationConfig::agents.at(1).bedroom, "Block2:MasterBedroom");
}
