// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "Building_Appliances.hpp"
#include "gtest/gtest.h"

class Test_Appliance_Group_CSV : public ::testing::Test {
 protected:
    Appliance_Group<Appliance_Generic_CSV> csv;
    std::vector<ConfigStructAppliance> AppliancesCSV;
    Contract_Negotiation local_negotiation;
    Contract_Negotiation neigh_negotiation;
    Contract_Negotiation global_negotiation;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Group_CSV::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles +
                                                  "DSM1house.xml");

  Configuration::setStepCount(-1);
  Utility::setSeed(0);
  ConfigStructAppliance a;
  a.fileSupply = testFiles + "PVBowler2013_365.csv";
  //a.fileDemand;
  a.id = 0;
  a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  a.costVector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  AppliancesCSV.push_back(a);
}

void Test_Appliance_Group_CSV::AfterConfiguration() {}

TEST_F(Test_Appliance_Group_CSV, power) {

  csv.setup(AppliancesCSV, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double supply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    csv.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);
    csv.clear();
  }
}


TEST_F(Test_Appliance_Group_CSV, powerSupply) {

  csv.setup(AppliancesCSV, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double supply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract c;
    c.supplied = 10;
    c.buildingID = 100;

    global_negotiation.submit(c);

    csv.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    if(power > c.supplied){
        EXPECT_NEAR(recieved, c.supplied, 0.01);
    } else if(power > 0){
        EXPECT_NEAR(recieved, power, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }
    csv.clear();
  }

}

TEST_F(Test_Appliance_Group_CSV, Appliances) {
  for(int i = 0; i < 5; i++) {
    ConfigStructAppliance a;
    if (i<3){
      a.fileSupply = testFiles + "PVBowler2013_365.csv";
    } else{
      a.fileDemand = testFiles + "PVBowler2013_365.csv";
    }
    a.id = i + 1;
    a.costVector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    AppliancesCSV.push_back(a);
  }
  for(int build = 0; build < 40; build++) {
      csv.setup(AppliancesCSV, build, "test_building_" + std::to_string(build) + "_");
  }
  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double supply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);


    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);


    csv.addGlobalContactsTo(&global_negotiation);


    double diff = global_negotiation.getDifference();
    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = std::abs(diff);
    m.suppliedCost = 0;
    m.requested = 0;
    if (diff > 0.0) {
      m.supplied = 0;
      m.suppliedCost = 0;
      m.requested = std::numeric_limits<double>::max();
    }
    m.suppliedLeft = m.supplied;
    global_negotiation.submit(m);

    global_negotiation.process();
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);

    EXPECT_NEAR(recieved, power, 0.01);
    if (diff < 0.0) {
      EXPECT_NEAR(diff, power - supply, 0.01);
    }
    csv.clear();
  }

}


TEST_F(Test_Appliance_Group_CSV, networkLevelLocal) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 1000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double localSupply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(localSupply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = localSupply;
    local_negotiation.submit(m);

    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    double supply = csv.getSupply();

    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    csv.addGlobalContactsTo(&global_negotiation);

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
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), power, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), power, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), localSupply, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), 0, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply + localSupply;
    csv.clear();
  }
  EXPECT_TRUE(totalSupply > 0);
}



TEST_F(Test_Appliance_Group_CSV, networkLevelNeighbourhood) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double localSupply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(localSupply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = localSupply;
    neigh_negotiation.submit(m);
    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    double neighSupply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(neighSupply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    csv.addGlobalContactsTo(&global_negotiation);

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
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    double supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, diff, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);

    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), 0, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), localSupply, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), neighSupply, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), 0, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply + localSupply + neighSupply;
    csv.clear();
  }
  EXPECT_TRUE(totalSupply > 0);
}

TEST_F(Test_Appliance_Group_CSV, networkLevelGrid) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 10000; i++) {
    Configuration::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double localSupply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(localSupply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    csv.localNegotiation(local_negotiation);
    local_negotiation.clear();

    csv.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    csv.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = csv.getPower();
    double neighSupply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(neighSupply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    csv.addGlobalContactsTo(&global_negotiation);

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = neighSupply;
    global_negotiation.submit(m);

    global_negotiation.process();
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    double supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), 0, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), localSupply, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), neighSupply, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), supply, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply + localSupply + neighSupply;
    csv.clear();
  }
  EXPECT_TRUE(totalSupply > 0);
}
