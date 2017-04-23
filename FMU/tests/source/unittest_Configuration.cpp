// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "gtest/gtest.h"

class Test_Configuration : public ::testing::Test {
 protected:
    int res;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Configuration::SetUp() {
  Configuration::reset();
}

void Test_Configuration::AfterConfiguration() {
}

TEST_F(Test_Configuration, Configuration) {
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).profile.size(), 24);
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).profile.size(), 24);
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).profile.at(0), "0.919825741,0.013844725,0.011501807,0.001339251,0.002836063,0.000551457,0.00416508,0.002205826,0.001921411,0.041808638");
}

TEST_F(Test_Configuration, Configuration1) {
  Configuration::parseConfiguration(testFiles + "/SimulationConfig1.xml");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).age, "age2");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).computer, "computer0");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).civstat, "civstat1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).unemp, "unemp0");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).retired, "retired1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).edtry, "edtry1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).famstat, "famstat3");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).sex, "sex2");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).age, "age2");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).computer, "computer0");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).civstat, "civstat1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).unemp, "unemp0");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).retired, "retired1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).edtry, "edtry1");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).famstat, "famstat3");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).sex, "sex2");
  EXPECT_EQ(Configuration::buildings[0].agents.at(0).bedroom, "Block2:MasterBedroom");
  EXPECT_EQ(Configuration::buildings[0].agents.at(1).bedroom, "Block2:MasterBedroom");
}

TEST_F(Test_Configuration, results) {
  Configuration::parseConfiguration(testFiles + "/results.xml");
  EXPECT_EQ(Configuration::outputRegexs.at(0), ".*");
  EXPECT_EQ(Configuration::outputRegexs.at(1), "Building1_Appliance1_.*");
}
