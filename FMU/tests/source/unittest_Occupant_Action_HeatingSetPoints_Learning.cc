// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.h"
#include "DataStore.h"
#include "SimulationTime.h"
#include "Occupant_Action_HeatingSetPoints_Learning.h"
#include "gtest/gtest.h"

class Test_Occupant_Action_HeatingSetPoints_Learning : public ::testing::Test {
 protected:
    Occupant_Action_HeatingSetPoints_Learning aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Occupant_Action_HeatingSetPoints_Learning::SetUp() {
  SimulationConfig::buildings[0].agents.clear();
  SimulationConfig::parseConfiguration(testFiles + "/SimulationConfig2.xml");

  SimulationConfig::setStepCount(0);
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.learnep = 0.8;
  DataStore::addVariable("Block1:KitchenZoneMeanAirTemperature");
  DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature", 18);
  SimulationTime::hour = 1;
  SimulationTime::month = 1;
  SimulationTime::hourOfDay = 1;
  SimulationTime::day = 1;
}

TEST_F(Test_Occupant_Action_HeatingSetPoints_Learning, Learn) {
  aal.setup(1, 1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen;
  z_Kitchen.setName(zs.name);
  z_Kitchen.setup(zs);
  activities.push_back(4);

  double heating;
  for (int i =0; i < 100000; i++) {
     aal.step(z_Kitchen, true);
     heating = aal.getResult();
     DataStore::addValueS("Block1:KitchenZoneMeanAirTemperature",
                        heating);
  }

  aal.step(z_Kitchen, true);
  //heating = aal.getResult();
//  EXPECT_NEAR(heating, 21, 0.1);
}
