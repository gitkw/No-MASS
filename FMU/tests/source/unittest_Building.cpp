// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.hpp"
#include "Configuration.hpp"
#include "DataStore.hpp"
#include "Building.hpp"
#include "gtest/gtest.h"

class Test_Building : public ::testing::Test {
 protected:
  virtual void SetUp();
};

void Test_Building::SetUp() {
    Configuration::reset();
    Configuration::RunLocation  = testFiles;
    Configuration::parseConfiguration(Configuration::RunLocation
      + "/SimulationConfig3.xml");
    Configuration::info.windows = false;
    Configuration::info.shading = false;
    Configuration::info.lights = false;

    Configuration::setStepCount(-1);
    DataStore::addVariable("Block1:Zone1ZoneMeanAirTemperature");
    DataStore::addVariable("Block1:Zone1ZoneAirRelativeHumidity");
    DataStore::addVariable("Block1:Zone1ZoneMeanRadiantTemperature");
    DataStore::addVariable("Block2:Zone1ZoneMeanAirTemperature");
    DataStore::addVariable("Block2:Zone1ZoneAirRelativeHumidity");
    DataStore::addVariable("Block2:Zone1ZoneMeanRadiantTemperature");

    DataStore::addValueS("Block1:Zone1ZoneMeanAirTemperature", 21);
    DataStore::addValueS("Block1:Zone1ZoneAirRelativeHumidity", 21);
    DataStore::addValueS("Block1:Zone1ZoneMeanRadiantTemperature", 21);
    DataStore::addValueS("Block2:Zone1ZoneMeanAirTemperature", 21);
    DataStore::addValueS("Block2:Zone1ZoneAirRelativeHumidity", 21);
    DataStore::addValueS("Block2:Zone1ZoneMeanRadiantTemperature", 21);
    Configuration::step();
}


TEST_F(Test_Building, one) {
    Building building;
    building.setup(Configuration::buildings[0]);

    EXPECT_TRUE(building.hasZone("Block1:Zone1"));
    EXPECT_FALSE(building.hasZone("Block2:Zone1"));
    EXPECT_TRUE(building.hasZone("out"));

    for (int i = 0; i < 100; i++) {
      building.step();
    }
    building.postprocess();
}

TEST_F(Test_Building, two) {
    std::vector<Building> buildings;

    for (ConfigStructBuilding b : Configuration::buildings) {
      buildings.push_back(Building());
      buildings.back().setup(b);
    }

    EXPECT_EQ(buildings.size(), 2);
    EXPECT_TRUE(buildings[0].hasZone("Block1:Zone1"));
    EXPECT_FALSE(buildings[0].hasZone("Block2:Zone1"));
    EXPECT_TRUE(buildings[0].hasZone("out"));
    EXPECT_FALSE(buildings[1].hasZone("Block1:Zone1"));
    EXPECT_TRUE(buildings[1].hasZone("Block2:Zone1"));
    EXPECT_TRUE(buildings[1].hasZone("out"));


    for (Building &building : buildings) {
      for (int i = 0; i < 100; i++) {
        building.step();
      }
    }
    for (Building &building : buildings) {
      building.postprocess();
    }
}

TEST_F(Test_Building, compareBool) {
  Building building;
  building.setup(Configuration::buildings[0]);
  EXPECT_TRUE(building.decisionBoolean(0.0, 0.1));
  EXPECT_FALSE(building.decisionBoolean(0.1, 0.0));
  int count = 0;
  for (int i = 0; i < 100; i++) {
    if (building.decisionBoolean(0.1, 0.1)) count++;
  }

  EXPECT_TRUE(count > 40);
  EXPECT_TRUE(count < 60);

}

TEST_F(Test_Building, compareDouble) {
  Building building;
  building.setup(Configuration::buildings[0]);
  std::vector<double> val;
  std::vector<double> power;

  val.push_back(0.1);
  power.push_back(0.1);
  EXPECT_NEAR(building.decisionDoubleVec(val, power, 0.1), 0.1, 0.001);

  val.push_back(0.2);
  power.push_back(0.2);
  EXPECT_NEAR(building.decisionDoubleVec(val, power, 0.2), 0.2, 0.001);

  val.push_back(0.3);
  power.push_back(0.2);
  EXPECT_NEAR(building.decisionDoubleVec(val, power, 0.1), 0.25, 0.001);

  val.push_back(0.0);
  power.push_back(0.5);
  EXPECT_NEAR(building.decisionDoubleVec(val, power, 0.2), 0.05, 0.001);

}
