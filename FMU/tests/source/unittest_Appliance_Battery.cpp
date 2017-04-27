// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "tests/Gen.hpp"

#include "Configuration.hpp"
#include "Appliance_Battery.hpp"

#include "gtest/gtest.h"

class Test_Appliance_Battery : public ::testing::Test {
 protected:

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Battery::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles + "SimulationConfig.xml");
  Configuration::setStepCount(-1);
  Configuration::FileLargeAppliance = testFiles + "AppliancesLarge.xml";

}

void Test_Appliance_Battery::AfterConfiguration() {
}

TEST_F(Test_Appliance_Battery, noRecived) {
  ConfigStructAppliance a;
  Appliance_Battery al;
  al.setup(a);
  al.setID(1);
  Configuration::step();
  double powerShortage = 1000;
  al.setPowerShortage(powerShortage);
  al.step();
  double supply = al.getSupply();
  powerShortage -= supply;
  EXPECT_NEAR(supply, 0, 0);
  EXPECT_NEAR(powerShortage, 1000, 0);
  for (int i = 1; i < 100000; i++) {
    Configuration::step();
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
  ConfigStructAppliance a;
  Appliance_Battery al;
    al.setup(a);
    al.setID(1);
  Configuration::step();
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
    Configuration::step();
    al.setPowerShortage(powerShortage);
    al.step();
    supply = al.getSupply();
    double requested = al.getPower();
    al.setReceived(requested);
    if(supply > 0){
      if (al.getStateOfCharge() > 0){
        if (supply != 1000){
         EXPECT_NEAR(supply, powerShortage, 0);
        }else {
            EXPECT_NEAR(supply, 1000, 0);
        }
      }
    } else {
      EXPECT_NEAR(supply, 0, 0);
    }
      powerShortage -= supply;
    al.clear();
  }
}


TEST_F(Test_Appliance_Battery, ReceivedNeigh) {
    ConfigStructAppliance a;
  Appliance_Battery al;
    al.setup(a);
    al.setID(1);
  al.setupSave();
  Configuration::step();
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
    Configuration::step();
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


TEST_F(Test_Appliance_Battery, one) {
  // create a battery
  ConfigStructAppliance a;
  a.batteryNeighbourhoodCharge = true;
  a.batteryNeighbourhoodDischarge = true;
  Appliance_Battery al;

  al.setup(a);
  al.setID(1);
  al.setupSave();
  Configuration::step();
  al.setPowerShortage(0);

  double received = 10;
  al.step();
  al.setReceived(received);
  al.saveLocal();
  al.stepNeighbourhood();
  al.saveNeighbourhood();
  al.setPower(0.0);
  al.setSupply(0.0);
  al.setSupplyCost(0.0);
  al.setReceivedCost(0.0);
  al.setSupplyLeft(0.0);
  al.saveGlobal();
  al.clear();

  EXPECT_NEAR(al.getLocalPower(), 1000, 0.01);
  EXPECT_NEAR(al.getLocalReceived(), received, 0.01);
  EXPECT_NEAR(al.getLocalSupply(), 0, 0.01);

  EXPECT_NEAR(al.getNeighbourhoodPower(), 990, 0.01);
  EXPECT_NEAR(al.getNeighbourhoodReceived(), 0, 0.01);
  EXPECT_NEAR(al.getNeighbourhoodSupply(), 0, 0.01);

  EXPECT_NEAR(al.getGridPower(), 0, 0.01);
  EXPECT_NEAR(al.getGridReceived(), 0, 0.01);
  EXPECT_NEAR(al.getGridSupply(), 0, 0.01);

}

TEST_F(Test_Appliance_Battery, two) {
  // create a battery
  ConfigStructAppliance a;

  a.batteryNeighbourhoodCharge = true;
  a.batteryNeighbourhoodDischarge = true;
  Appliance_Battery al;

  al.setup(a);
  al.setID(1);
  al.setupSave();
  //charge battery
  Configuration::step();
  al.setPowerShortage(0);

  double received = 100;
  al.step();
  al.setReceived(received);
  al.saveLocal();
  al.setPowerShortage(0);
  al.stepNeighbourhood();
  al.saveNeighbourhood();
  al.setPower(0.0);
  al.setSupply(0.0);
  al.setSupplyCost(0.0);
  al.setReceivedCost(0.0);
  al.setSupplyLeft(0.0);
  al.saveGlobal();
  al.clear();

  for (int i =0; i <1000; i++){
    //charge battery
    Configuration::step();
    al.setPowerShortage(90);

    received = 0;
    al.step();
    double supply = al.getSupply();
    al.setReceived(received);
    al.saveLocal();
    al.setPowerShortage(0);
    al.stepNeighbourhood();
    al.setSupplyLeft(0.0);
    al.saveNeighbourhood();
    al.setPower(0.0);
    al.setSupply(0.0);
    al.setSupplyCost(0.0);
    al.setReceivedCost(0.0);
    al.saveGlobal();
    al.clear();
    if(supply > 0 && al.getStateOfCharge() > 0) {
        EXPECT_NEAR(al.getLocalPower(), 0, 0.01);
        EXPECT_NEAR(al.getLocalReceived(), 00, 0.01);
        EXPECT_NEAR(al.getLocalSupply(), 0, 0.01);

        EXPECT_NEAR(al.getNeighbourhoodPower(), 0, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodReceived(), 00, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodSupply(), 90, 0.01);

        EXPECT_NEAR(al.getGridPower(), 0, 0.01);
        EXPECT_NEAR(al.getGridReceived(), 0, 0.01);
        EXPECT_NEAR(al.getGridSupply(), 0, 0.01);
    }
  }
}


TEST_F(Test_Appliance_Battery, three) {
  // create a battery
  ConfigStructAppliance a;

    a.batteryNeighbourhoodCharge = true;
    a.batteryNeighbourhoodDischarge = true;
  Appliance_Battery al;

  al.setup(a);
  al.setID(1);
  al.setupSave();
  //charge battery
  Configuration::step();
  al.setPowerShortage(0);

  double received = 1000;
  al.step();
  al.setReceived(received);
  al.saveLocal();
  al.setPowerShortage(0);
  al.stepNeighbourhood();
  al.saveNeighbourhood();
  al.setPower(0.0);
  al.setSupply(0.0);
  al.setSupplyCost(0.0);
  al.setReceivedCost(0.0);
  al.setSupplyLeft(0.0);
  al.saveGlobal();
  al.clear();

  for (int i =0; i <1000; i++){
    //charge battery
    Configuration::step();
    al.setPowerShortage(10);

    received = 0;
    al.step();
    double supply = al.getSupply();
    al.setReceived(received);
    al.setSupplyLeft(5);
    al.saveLocal();
    al.setPowerShortage(0);
    al.stepNeighbourhood();
    al.saveNeighbourhood();
    al.setPower(0.0);
    al.setSupply(0.0);
    al.setSupplyCost(0.0);
    al.setReceivedCost(0.0);
    al.setSupplyLeft(0.0);
    al.saveGlobal();
    al.clear();
    if(supply > 0) {
        EXPECT_NEAR(al.getLocalPower(), 0, 0.01);
        EXPECT_NEAR(al.getLocalReceived(), 00, 0.01);
        EXPECT_NEAR(al.getLocalSupply(), 5, 0.01);

        EXPECT_NEAR(al.getNeighbourhoodPower(), 0, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodReceived(), 00, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodSupply(), 0, 0.01);

        EXPECT_NEAR(al.getGridPower(), 0, 0.01);
        EXPECT_NEAR(al.getGridReceived(), 0, 0.01);
        EXPECT_NEAR(al.getGridSupply(), 0, 0.01);
    }
  }

}



TEST_F(Test_Appliance_Battery, threee) {
  // create a battery
  ConfigStructAppliance a;

    a.batteryNeighbourhoodCharge = true;
    a.batteryNeighbourhoodDischarge = true;
  Appliance_Battery al;

  al.setup(a);
  al.setID(1);
  al.setupSave();
  //charge battery
  Configuration::step();
  al.setPowerShortage(0);

  double received = 1000;
  al.step();
  al.setReceived(received);
  al.saveLocal();
  al.setPowerShortage(0);
  al.stepNeighbourhood();
  al.saveNeighbourhood();
  al.setPower(0.0);
  al.setSupply(0.0);
  al.setSupplyCost(0.0);
  al.setReceivedCost(0.0);
  al.setSupplyLeft(0.0);
  al.saveGlobal();
  al.clear();

  for (int i =0; i <1000; i++){
    //charge battery
    Configuration::step();
      double shortage = 10;
    al.setPowerShortage(shortage);

    received = 0;
    al.step();
    double supply = al.getSupply();
    al.setReceived(received);
    al.setSupplyLeft(0);
    al.saveLocal();
    al.setPowerShortage(10);
    al.stepNeighbourhood();
    double supplyNeigh = al.getSupply() - supply;
    al.setSupplyLeft(0);
    al.saveNeighbourhood();
    al.setPower(0.0);
    al.setSupply(0.0);
    al.setSupplyCost(0.0);
    al.setReceivedCost(0.0);
    al.setSupplyLeft(0.0);
    al.saveGlobal();
    al.clear();
    if(supplyNeigh > 0) {
        EXPECT_NEAR(al.getLocalReceived(), 00, 0.01);
        EXPECT_NEAR(al.getLocalSupply(), supply, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodReceived(), 00, 0.01);
        EXPECT_NEAR(al.getNeighbourhoodSupply(), supplyNeigh, 0.01);
        EXPECT_NEAR(al.getGridPower(), 0, 0.01);
        EXPECT_NEAR(al.getGridReceived(), 0, 0.01);
        EXPECT_NEAR(al.getGridSupply(), 0, 0.01);
    }
  }

}


TEST_F(Test_Appliance_Battery, four) {
    Configuration::info.timeStepsPerHour = 1;
  // create a battery
  ConfigStructAppliance a;
  a.batteryNeighbourhoodCharge = true;
  a.batteryNeighbourhoodDischarge = true;
  Appliance_Battery al;

  al.setup(a);
  al.setID(1);
  al.setupSave();
  //charge battery
  Configuration::step();


  double received = 1000;
  al.setPowerShortage(0);
  al.step();
  al.setReceived(received);
  al.saveLocal();


  while (al.getSupply() == 0) {
    al.clear();
    Configuration::step();
    al.setPowerShortage(100);

    al.step();
    al.setReceived(0);
    al.setSupplyLeft(0);
    al.saveLocal();
  }



  EXPECT_NEAR(al.getLocalReceived(), 00, 0.01);
  EXPECT_NEAR(al.getLocalSupply(), 100, 0.01);


}
