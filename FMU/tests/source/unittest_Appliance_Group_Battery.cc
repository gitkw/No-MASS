// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.h"
#include "SimulationConfig.h"
#include "Building_Appliances.h"
#include "Appliance_Group_Battery.h"
#include "gtest/gtest.h"


class Test_Appliance_Group_Battery : public ::testing::Test {
 protected:
    Appliance_Group_Battery<Appliance_Battery> batt;
    std::vector<applianceStruct> AppliancesLarge;
    Contract_Negotiation local_negotiation;
    Contract_Negotiation neigh_negotiation;
    Contract_Negotiation global_negotiation;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Group_Battery::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles +
                                                  "DSM1house.xml");
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";
  SimulationConfig::FolderSmallAppliance = testFiles + "SmallAppliances/";
  SimulationConfig::setStepCount(-1);
  Utility::setSeed(0);
  applianceStruct a;

  a.id = 0;
  a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    a.batteryNeighbourhoodCharge = true;
    a.batteryNeighbourhoodDischarge = true;
  AppliancesLarge.push_back(a);
}

void Test_Appliance_Group_Battery::AfterConfiguration() {}

TEST_F(Test_Appliance_Group_Battery, power) {

  batt.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
    batt.step(&local_negotiation);
    double power = batt.getPower();
    double supply = batt.getSupply();
    double recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    batt.localNegotiation(local_negotiation);
    local_negotiation.clear();

    batt.addGlobalContactsTo(&neigh_negotiation);

    batt.neighbourhoodNegotiationBattery(&neigh_negotiation);

    neigh_negotiation.process();
    batt.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = batt.getPower();
    supply = batt.getSupply();
    recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    batt.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    batt.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = batt.getPower();
    supply = batt.getSupply();
    recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);
  }
}



TEST_F(Test_Appliance_Group_Battery, neigh) {

  batt.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {


      SimulationConfig::step();

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 100;
    m.suppliedLeft = 100;
    m.suppliedCost = 0;
    m.requested = 0;
    local_negotiation.submit(m);

    batt.step(&local_negotiation);
    double power = batt.getPower();
      double supply = batt.getSupply();
      double recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    batt.localNegotiation(local_negotiation);
    local_negotiation.clear();

    batt.addGlobalContactsTo(&neigh_negotiation);

    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = 100;
    m.suppliedLeft = 0;
    m.received = 0;
    neigh_negotiation.submit(m);

    batt.neighbourhoodNegotiationBattery(&neigh_negotiation);

    neigh_negotiation.process();
    batt.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();

    power = batt.getPower();
    supply = batt.getSupply();
    recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(0 >= 0);


    batt.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    batt.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = batt.getPower();
    supply = batt.getSupply();
    recieved = batt.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);

    if(power > 0 && power < 100){
      EXPECT_NEAR(recieved, power, 0);
    }else if (power > 0 && power > 100 && supply == 0){
      EXPECT_NEAR(recieved, 100, 0);
    }else {
      //EXPECT_NEAR(recieved, 0, 0);
    }
  }
}
