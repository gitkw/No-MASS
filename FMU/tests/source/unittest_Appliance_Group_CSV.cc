// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.h"
#include "SimulationConfig.h"
#include "Building_Appliances.h"
#include "gtest/gtest.h"

/*
struct applianceStruct {
    std::string name;
    int id;
    std::vector<double> priority;
    std::vector<double> timeRequired
    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double cost;
    double epsilon = 0.1;   // probability of a random action selection
    double alpha = 0.3;     // learning rate
    double gamma = 0.1;     // discount factor (how soon do you care)
    bool update = false;
    std::vector<int> activities;
};
*/

class Test_Appliance_Group_CSV : public ::testing::Test {
 protected:
    Appliance_Group_CSV csv;
    std::vector<applianceStruct> AppliancesCSV;
    Contract_Negotiation local_negotiation;
    Contract_Negotiation neigh_negotiation;
    Contract_Negotiation global_negotiation;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Group_CSV::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles +
                                                  "DSM1house.xml");


  SimulationConfig::setStepCount(-1);
  Utility::setSeed(0);
  applianceStruct a;
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
    SimulationConfig::step();
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
    csv.reset();
  }
}


TEST_F(Test_Appliance_Group_CSV, powerSupply) {

  csv.setup(AppliancesCSV, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
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
    csv.reset();
  }

}

TEST_F(Test_Appliance_Group_CSV, Appliances) {
  for(int i = 0; i < 5; i++) {
    applianceStruct a;
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
    SimulationConfig::step();
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
    if (diff < 0.0) {
      Contract m;
      m.id = -1;
      m.buildingID = -1;
      m.supplied = std::abs(diff);
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

    EXPECT_NEAR(recieved, power, 0.01);
    if (diff < 0.0) {
      EXPECT_NEAR(diff, power - supply, 0.01);
    }
    csv.reset();
  }

}


TEST_F(Test_Appliance_Group_CSV, networkLevelLocal) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 1000; i++) {
    SimulationConfig::step();
    csv.step(&local_negotiation);
    double power = csv.getPower();
    double supply = csv.getSupply();
    double recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = supply;
    local_negotiation.submit(m);

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
    csv.reset();

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), power, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), power, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), supply, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), 0, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply;
  }
  EXPECT_TRUE(totalSupply > 0);
}



TEST_F(Test_Appliance_Group_CSV, networkLevelNeighbourhood) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
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

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = supply;
    neigh_negotiation.submit(m);
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
    csv.reset();

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);

    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), 0, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), supply, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), supply, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), 0, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), 0, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply;
  }
  EXPECT_TRUE(totalSupply > 0);
}

TEST_F(Test_Appliance_Group_CSV, networkLevelGrid) {
  double totalSupply = 0;
  csv.setup(AppliancesCSV, 0, "test_building_");
  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
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

    Contract m;
    m.id = -1;
    m.buildingID = -1;
    m.supplied = 0;
    m.suppliedCost = 0;
    m.requested = supply;
    global_negotiation.submit(m);

    global_negotiation.process();
    csv.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = csv.getPower();
    supply = csv.getSupply();
    recieved = csv.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(power, 0, 0);

    csv.reset();

    Appliance_Generic_CSV a = csv.getApplianceAt(0, 0);
    EXPECT_NEAR(a.getLocalPower(), 0, 0.01);
    EXPECT_NEAR(a.getLocalReceived(), 0, 0.01);
    EXPECT_NEAR(a.getLocalSupply(), 0, 0);
    EXPECT_NEAR(a.getLocalSupplyLeft(), supply, 0);
    EXPECT_NEAR(a.getNeighbourhoodPower(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodReceived(), 0, 0.01);
    EXPECT_NEAR(a.getNeighbourhoodSupply(), 0, 0);
    EXPECT_NEAR(a.getNeighbourhoodSupplyLeft(), supply, 0);
    EXPECT_NEAR(a.getGridPower(), 0, 0.01);
    EXPECT_NEAR(a.getGridReceived(), 0, 0.01);
    EXPECT_NEAR(a.getGridSupply(), supply, 0);
    EXPECT_NEAR(a.getGridSupplyLeft(), 0, 0);
    totalSupply += supply;
  }
  EXPECT_TRUE(totalSupply > 0);
}
