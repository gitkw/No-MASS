// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "Building_Appliances.hpp"
#include "gtest/gtest.h"

class Test_Appliance_Group_Large : public ::testing::Test {
 protected:
    Appliance_Group<Appliance_Large> large;
    std::vector<ConfigStructAppliance> AppliancesLarge;
    Contract_Negotiation local_negotiation;
    Contract_Negotiation neigh_negotiation;
    Contract_Negotiation global_negotiation;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Group_Large::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles +
                                                  "DSM1house.xml");
  Configuration::FileLargeAppliance = testFiles + "AppliancesLarge.xml";
  Configuration::FolderSmallAppliance = testFiles + "SmallAppliances/";
  Configuration::setStepCount(-1);
  Utility::setSeed(0);
  ConfigStructAppliance a;
  a.name = "test_app_";
  a.id = 0;
  a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  AppliancesLarge.push_back(a);
}

void Test_Appliance_Group_Large::AfterConfiguration() {}

TEST_F(Test_Appliance_Group_Large, power) {

  large.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);
    large.clear();
  }
}


TEST_F(Test_Appliance_Group_Large, powerSupply) {

  large.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract c;
    c.supplied = 10;
    c.suppliedLeft = 10;
    c.buildingID = 100;

    global_negotiation.submit(c);

    large.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > c.supplied){
        EXPECT_NEAR(recieved, c.supplied, 0.01);
    } else if(power > 0){
        EXPECT_NEAR(recieved, power, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }
    large.clear();
  }

}

TEST_F(Test_Appliance_Group_Large, Appliances) {
  for(int i = 0; i < 5; i++) {
    ConfigStructAppliance a;
    a.name = "test_app_" + std::to_string(i+1) + "_";
    a.id = i + 1;
    a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    AppliancesLarge.push_back(a);
  }
  for(int build = 0; build < 40; build++) {
      large.setup(AppliancesLarge, build, "test_building_" + std::to_string(build) + "_");
  }
  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);

    double diff = global_negotiation.getDifference();
    if (diff < 0.0) {
      Contract m;
      m.id = -1;
      m.buildingID = -1;
      m.supplied = std::abs(diff);
      m.suppliedLeft = std::abs(diff);
      m.suppliedCost = 0;
      m.requested = 0;
      global_negotiation.submit(m);
    }
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > 0){
        EXPECT_NEAR(-recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }
    large.clear();
  }

}


TEST_F(Test_Appliance_Group_Large, networkLevelLocal) {

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = power;
    m.suppliedCost = 0;
    m.requested = 0;
    local_negotiation.submit(m);


    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);

    double diff = global_negotiation.getDifference();
    if (diff < 0.0) {
      Contract m;
      m.id = -1;
      m.buildingID = -1;
      m.supplied = 0;
      m.suppliedCost = 0;
      m.requested = 0;
      global_negotiation.submit(m);
    }
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }

    Appliance_Large a = large.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), power, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), power, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    large.clear();
  }
}

TEST_F(Test_Appliance_Group_Large, networkLevelNeighbourhood) {

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = power;
    m.suppliedCost = 0;
    m.requested = 0;
    neigh_negotiation.submit(m);
    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);

    double diff = global_negotiation.getDifference();
    if (diff < 0.0) {
      Contract m;
      m.id = -1;
      m.buildingID = -1;
      m.supplied = 0;
      m.suppliedCost = 0;
      m.requested = 0;
      global_negotiation.submit(m);
    }
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }

    Appliance_Large a = large.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodPower(), power, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), power, 0.01);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    large.clear();
  }
}

TEST_F(Test_Appliance_Group_Large, networkLevelGrid) {

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);

    double diff = global_negotiation.getDifference();
    if (diff < 0.0) {
      Contract m;
      m.id = -1;
      m.buildingID = -1;
      m.supplied = power;
      m.suppliedCost = 0;
      m.requested = 0;
      global_negotiation.submit(m);
    }
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }

    Appliance_Large a = large.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridPower(), power, 0.01);
    EXPECT_NEAR(a.getGridReceived(), power, 0.01);
    large.clear();
  }
}
