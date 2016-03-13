// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "DataStore.h"
#include "Agent_Action_Learning.h"
#include "gtest/gtest.h"

class Test_Agent_Action_Learning : public ::testing::Test {
 protected:
    Agent_Action_Learning aal;
    std::vector<double> activities;
    virtual void SetUp();
};

void Test_Agent_Action_Learning::SetUp() {
  SimulationConfig::agents.clear();
  SimulationConfig::parseConfiguration("../tests/Files/SimulationConfig2.xml");

  SimulationConfig::stepCount = 0;
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.learnep = 0.8;
  DataStore::addValue("Block1:KitchenZoneMeanAirTemperature", 18);
  DataStore::addVariable("hour");
  DataStore::addVariable("month");
  DataStore::addVariable("hourOfDay");
  DataStore::addValue("hour", 1);
  DataStore::addValue("month", 1);
  DataStore::addValue("hourOfDay", 1);
}

TEST_F(Test_Agent_Action_Learning, Learn) {
  aal.setup(1, 1);

  ZoneStruct zs;
  zs.name = "Block1:Kitchen";
  zs.id = 1;
  Building_Zone z_Kitchen("", zs);
  activities.push_back(4);

  double heating;
  for (int i =0; i < 100000; i++) {
     aal.step(z_Kitchen, true, false, activities);
     heating = aal.getResult();
     DataStore::addValue("Block1:KitchenZoneMeanAirTemperature",
                        heating);
  }

  aal.step(z_Kitchen, true, false, activities);
  heating = aal.getResult();
//  ASSERT_NEAR(heating, 21, 0.1);
}
