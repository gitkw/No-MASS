// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Shades.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Shades : public ::testing::Test {
 protected:
    Occupant_Action_Shades aas;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Shades::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenDaylightingReferencePoint1Illuminance", 2);
  DataStore::addValue("EnvironmentSiteExteriorHorizontalSkyIlluminance", 200);
}