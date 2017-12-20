// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "Appliance_Generic_CSV.hpp"
#include "Utility.hpp"
#include "gtest/gtest.h"

/*
struct ConfigStructAppliance {
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

class Test_Appliance_Generic_CSV : public ::testing::Test {
 protected:
    Appliance_Generic_CSV csv;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Generic_CSV::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles + "DSM1house.xml");
  Configuration::setStepCount(-1);
  Utility::setSeed(0);
  ConfigStructAppliance a;
  a.fileSupply = testFiles + "PVBowler2013_365.csv";
  a.fileDemand = testFiles + "HeatingPower.csv";
  a.id = 0;
  a.costVector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  csv.setID(a.id);
  csv.setHoulyPriority(a.priority);
  csv.setHourlyCost(a.costVector);
  csv.setBuildingID(0);
  csv.setFileDemand(a.fileDemand);
  csv.setFileSupply(a.fileSupply);
  csv.setup(a);
  csv.setIDString("test_building_" + std::to_string(0) + "_" + std::to_string(a.id));
  csv.setupSave();

}

void Test_Appliance_Generic_CSV::AfterConfiguration() {}

TEST_F(Test_Appliance_Generic_CSV, power) {

  for(int i = 0; i < 70157; i++) {
    Configuration::step();
    csv.step();
    csv.setReceivedCost(0);
    double power = csv.getPower();
    double supply = csv.getSupply();

    if(supply == 0 || power == 0) continue;

    double receivedlocal = supply;
    if (power - supply <= 0) {
      receivedlocal = power;
    }

    csv.setReceived(receivedlocal);
    csv.setSupplyLeft(supply - receivedlocal);
    csv.saveLocal();

    double receivedNeigh = 0;
    if (receivedlocal < power && supply > receivedlocal + receivedNeigh){
        receivedNeigh = Utility::randomDouble(0,power-receivedlocal);
    }

    csv.setReceived(receivedlocal + receivedNeigh);
    csv.setSupplyLeft(supply - (receivedlocal + receivedNeigh));
    csv.saveNeighbourhood();

    double receivedGrid = 0;
    if (receivedlocal + receivedNeigh < power && supply > receivedlocal + receivedNeigh){
        receivedGrid = Utility::randomDouble(0,power-receivedlocal);
    }

    csv.setReceived(receivedlocal + receivedNeigh + receivedGrid);
    csv.setSupplyLeft(supply - (receivedlocal + receivedNeigh + receivedGrid));

    csv.saveGlobal();

    power = csv.getPower();
    supply = csv.getSupply();
    //double recieved = csv.getReceived();

    double left = supply - power;
    if(supply - power < 0) left = 0;
    EXPECT_NEAR(csv.getLocalPower(), power, 0.01);
    EXPECT_NEAR(csv.getLocalReceived(), receivedlocal, 0.01);
    EXPECT_NEAR(csv.getLocalSupply(), receivedlocal, 0.01);
    EXPECT_NEAR(csv.getLocalSupplyLeft(), supply - receivedlocal, 0.01);
    EXPECT_NEAR(csv.getNeighbourhoodPower(), power - receivedlocal, 0.01);
    EXPECT_NEAR(csv.getNeighbourhoodReceived(), receivedNeigh, 0.01);
    EXPECT_NEAR(csv.getNeighbourhoodSupply(), receivedNeigh, 0.01);
    EXPECT_NEAR(csv.getNeighbourhoodSupplyLeft(), supply - (receivedlocal + receivedNeigh), 0.01);
    EXPECT_NEAR(csv.getGridPower(), power - receivedlocal - receivedNeigh, 0.01);
    EXPECT_NEAR(csv.getGridReceived(), receivedGrid, 0.01);
    EXPECT_NEAR(csv.getGridSupply(), receivedGrid, 0);
    EXPECT_NEAR(csv.getGridSupplyLeft(), supply - (receivedlocal + receivedNeigh + receivedGrid), 0);

    EXPECT_TRUE(csv.getLocalPower() >=0);
    EXPECT_TRUE(csv.getLocalReceived() >=0);
    EXPECT_TRUE(csv.getLocalSupply() >=0);
    EXPECT_TRUE(csv.getLocalSupplyLeft() >=0);
    EXPECT_TRUE(csv.getNeighbourhoodPower() >=0);
    EXPECT_TRUE(csv.getNeighbourhoodReceived() >=0);
    EXPECT_TRUE(csv.getNeighbourhoodSupply() >=0);
    EXPECT_TRUE(csv.getNeighbourhoodSupplyLeft() >=0);
    EXPECT_TRUE(csv.getGridPower() >=0);
    EXPECT_TRUE(csv.getGridReceived() >=0);
    EXPECT_TRUE(csv.getGridSupply() >=0);
    EXPECT_TRUE(csv.getGridSupplyLeft() >=0);
    csv.clear();
  }

}
