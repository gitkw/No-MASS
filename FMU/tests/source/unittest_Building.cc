// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.h"
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building.h"
#include "gtest/gtest.h"

class Test_Building : public ::testing::Test {
 protected:
  virtual void SetUp();
};

void Test_Building::SetUp() {
    SimulationConfig::reset();
    SimulationConfig::RunLocation  = testFiles;
    SimulationConfig::parseConfiguration(SimulationConfig::RunLocation
      + "/SimulationConfig3.xml");
    SimulationConfig::info.windows = false;
    SimulationConfig::info.shading = false;
    SimulationConfig::info.lights = false;

    SimulationConfig::setStepCount(-1);
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
    SimulationConfig::step();
}


TEST_F(Test_Building, one) {
    Building building;
    building.setup(SimulationConfig::buildings[0]);

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

    for (buildingStruct b : SimulationConfig::buildings) {
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

    std::cout << "hello" << std::endl;

    for (Building &building : buildings) {
      for (int i = 0; i < 100; i++) {
        building.step();
      }
    }

    std::cout << "hello" << std::endl;
    for (Building &building : buildings) {
      building.postprocess();
    }
}
