// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Lights.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Action_Lights : public ::testing::Test {
 protected:
    Occupant_Action_Lights aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_Lights::SetUp() {
  Configuration::reset();
  Configuration::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  Configuration::setStepCount(0);
  Configuration::info.windows = false;
  Configuration::info.shading = false;
  Configuration::info.lights = false;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addVariable("Block1:KitchenZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:KitchenZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("Block1:KitchenDaylightingReferencePoint1Illuminance");

  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 0);
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addValueS("Block1:KitchenZoneAirRelativeHumidity", 18);
  DataStore::addValueS("Block1:KitchenZoneMeanRadiantTemperature", 18);
  DataStore::addValueS("Block1:KitchenDaylightingReferencePoint1Illuminance", 1);
}
