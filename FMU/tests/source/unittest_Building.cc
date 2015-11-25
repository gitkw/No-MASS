// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Building.h"
#include "gtest/gtest.h"

TEST(Building, one) {
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
    SimulationConfig::step();

    Building building;
    building.setup(SimulationConfig::buildings[0]);
    for (int i = 0; i < 100; i++) {
      building.step();
    }
    building.postprocess();
}

TEST(Building, two) {
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

    std::vector<Building> buildings;

    for (buildingStruct b : SimulationConfig::buildings) {
      buildings.push_back(Building());
      buildings.back().setup(b);
    }
    for (Building &building : buildings) {
      for (int i = 0; i < 100; i++) {
        building.step();
      }
    }
    for (Building &building : buildings) {
      building.postprocess();
    }
}
