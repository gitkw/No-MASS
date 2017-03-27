// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "tests/Gen.h"

#include "SimulationConfig.h"
#include "Appliance_Battery.h"

#include "gtest/gtest.h"

class Test_Appliance_Battery : public ::testing::Test {
 protected:

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Battery::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "SimulationConfig.xml");
  SimulationConfig::setStepCount(-1);
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";

}

void Test_Appliance_Battery::AfterConfiguration() {
}

TEST_F(Test_Appliance_Battery, noRecived) {
  Appliance_Battery al;
  al.setID(1);
  al.setup();
  SimulationConfig::step();
  double powerShortage = 1000;
  al.setPowerShortage(powerShortage);
  al.step();
  double supply = al.getSupply();
  powerShortage -= supply;
  EXPECT_NEAR(supply, 0, 0);
  EXPECT_NEAR(powerShortage, 1000, 0);
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.setPowerShortage(powerShortage);
    al.step();
    supply = al.getSupply();
    powerShortage -= supply;
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(powerShortage, 1000, 0);
    al.clear();
  }
}


TEST_F(Test_Appliance_Battery, Received) {
  Appliance_Battery al;
  al.setID(1);
  al.setup();
  SimulationConfig::step();
  double powerShortage = 100000;
  al.setPowerShortage(powerShortage);
  al.step();
  double supply = al.getSupply();
  double requested = al.getPower();
  al.setReceived(requested);
  powerShortage -= supply;
  EXPECT_NEAR(supply, 0, 0);
  EXPECT_NEAR(powerShortage, 100000, 0);
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.setPowerShortage(powerShortage);
    al.step();
    supply = al.getSupply();
    double requested = al.getPower();
    al.setReceived(requested);
    powerShortage -= supply;
    if(supply > 0){
      EXPECT_NEAR(supply, 1000, 0);
    } else {
      EXPECT_NEAR(supply, 0, 0);
    }
    al.clear();
  }
}


TEST_F(Test_Appliance_Battery, ReceivedNeigh) {
  Appliance_Battery al;
  al.setID(1);
  al.setup();
  al.setupSave();
  SimulationConfig::step();
  double powerShortage = 100000;
  al.setPowerShortage(powerShortage);
  al.step();
  double supply = al.getSupply();
  double requested = al.getPower();
  al.setReceived(requested);
  powerShortage -= supply;
  EXPECT_NEAR(supply, 0, 0);
  EXPECT_NEAR(powerShortage, 100000, 0);
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.setPowerShortage(0);
    al.step();
    supply = al.getSupply();
    double requested = al.getPower();
    al.setReceived(requested);
    powerShortage -= supply;


    al.saveLocal();

    EXPECT_NEAR(supply, 0, 0);
    al.setPowerShortage(powerShortage);
    al.stepNeighbourhood();

    supply = al.getSupply();
    if(supply > 0){
      EXPECT_NEAR(supply, 1000, 0);
    } else {
      EXPECT_NEAR(supply, 0, 0);
    }
    al.saveNeighbourhood();
    al.clear();


    EXPECT_NEAR(al.getLocalPower(), requested, 0.01);
    EXPECT_NEAR(al.getLocalReceived(), requested, 0.01);
    EXPECT_NEAR(al.getLocalSupply(), 0, 0.01);

    EXPECT_NEAR(al.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(al.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(al.getNeighbourhoodSupply(), supply, 0.01);

    EXPECT_NEAR(al.getGridPower(), 0, 0.01);
    EXPECT_NEAR(al.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(al.getGridSupply(), 0, 0.01);


  }
}
