// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Occupant_Action_Lights.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_Lights : public ::testing::Test {
 protected:
    Occupant_Action_Lights aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Lights::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("Block1:KitchenDaylightingReferencePoint1Illuminance");

  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValue("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValue("Block1:KitchenZoneMeanRadiantTemperature", 18);
  DataStore::addValue("Block1:KitchenDaylightingReferencePoint1Illuminance", 1);
}
