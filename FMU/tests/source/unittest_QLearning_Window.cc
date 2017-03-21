// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>
#include <fstream>

#include "DataStore.h"
#include "QLearning_Occupant.h"

#include "Building_Zone.h"
#include "QLearning.h"
#include "gtest/gtest.h"

class Test_QLearning_Window : public ::testing::Test {
 protected:
    QLearning_Occupant ql;
    virtual void SetUp();
};

void Test_QLearning_Window::SetUp() {
  ql.setStates(31);
  ql.setActions(2);
  SimulationConfig::info.learnep = 0.8;
  SimulationConfig::info.learnupdate = true;
  DataStore::addVariable("hour");
  DataStore::addVariable("month");
  DataStore::addVariable("hourOfDay");
  DataStore::addValueS("hour", 1);
  DataStore::addValueS("month", 1);
  DataStore::addValueS("hourOfDay", 1);
}

TEST_F(Test_QLearning_Window, learn) {
    ql.setId(1);
    ql.setFilename("window-1-");
    ql.setEpsilon(0.1);
    ql.setUpdate(1);
    ql.setup();

    ZoneStruct zs;
    zs.name = "Block1:Zone1";
    zs.id = 1;
    Building_Zone z_Kitchen;
    z_Kitchen.setName(zs.name);
    z_Kitchen.setActive(true);
    z_Kitchen.setup(zs);

    double result = 1;
    for (int i =0; i < 100000; i++) {
      ql.setState(21);
      if (result > 0) {
        ql.setReward(1);
      } else {
        ql.setReward(-0.1);
      }
      result = ql.learn();
    }

    result = ql.learn();
    EXPECT_NEAR(result, 1, 0.1);
    ql.printQ();

    ql.setEpsilon(0.1);
    ql.setup();

    result = ql.learn();
    EXPECT_NEAR(result, 1, 0.1);

    for (int i =0; i < 100000; i++) {
      ql.setState(22);
      if (result > 0) {
        ql.setReward(-0.1);
      } else {
        ql.setReward(1);
      }
      result = ql.learn();
    }

    EXPECT_NEAR(result, 0, 0.1);
    ql.printQ();
}
