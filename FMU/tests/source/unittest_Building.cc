// Copyright 2015 Jacob Chapman

#include <limits.h>
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
    SimulationConfig::FmuLocation = "tests/Files";
    SimulationConfig::parseConfiguration(SimulationConfig::FmuLocation
      + "/SimulationConfig3.xml");
    SimulationConfig::info.windows = false;
    SimulationConfig::info.shading = false;
    SimulationConfig::info.lights = false;

    SimulationConfig::stepCount = -1;
    DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 21);
    DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 21);
    DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 21);
    DataStore::addValue("Block2:Zone1ZoneMeanAirTemperature", 21);
    DataStore::addValue("Block2:Zone1ZoneAirRelativeHumidity", 21);
    DataStore::addValue("Block2:Zone1ZoneMeanRadiantTemperature", 21);
    SimulationConfig::step();
}


TEST_F(Test_Building, one) {
    Building building;
    building.setup(SimulationConfig::buildings[0]);

    ASSERT_EQ(building.hasZone("Block1:Zone1"), true);
    ASSERT_EQ(building.hasZone("Block2:Zone1"), false);
    ASSERT_EQ(building.hasZone("Out"), true);

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
    ASSERT_EQ(buildings[0].hasZone("Block1:Zone1"), true);
    ASSERT_EQ(buildings[0].hasZone("Block2:Zone1"), false);
    ASSERT_EQ(buildings[0].hasZone("Out"), true);
    ASSERT_EQ(buildings[1].hasZone("Block1:Zone1"), false);
    ASSERT_EQ(buildings[1].hasZone("Block2:Zone1"), true);
    ASSERT_EQ(buildings[1].hasZone("Out"), true);

    for (Building &building : buildings) {
      for (int i = 0; i < 100; i++) {
        building.step();
      }
    }

    for (Building &building : buildings) {
      building.postprocess();
    }
}
