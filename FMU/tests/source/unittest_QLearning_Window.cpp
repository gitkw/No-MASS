// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>
#include <fstream>

#include "DataStore.hpp"
#include "QLearning.hpp"

#include "Building_Zone.hpp"
#include "QLearning.hpp"
#include "gtest/gtest.h"

class Test_QLearning_Window : public ::testing::Test {
 protected:
    QLearning ql;
    virtual void SetUp();
};

void Test_QLearning_Window::SetUp() {
  ql.setStates(31);
  ql.setActions(2);
  Configuration::info.learnep = 0.8;
  Configuration::info.learnupdate = true;
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

    ConfigStructZone zs;
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
        ql.learn();
      result = ql.getAction();
    }

    ql.learn();
    result = ql.getAction();
    EXPECT_NEAR(result, 1, 0.1);
    ql.printQ();

    ql.setEpsilon(0.1);
    ql.setup();

    ql.learn();
    result = ql.getAction();
    EXPECT_NEAR(result, 1, 0.1);

    for (int i =0; i < 100000; i++) {
      ql.setState(22);
      if (result > 0) {
        ql.setReward(-0.1);
      } else {
        ql.setReward(1);
      }
        ql.learn();
      result = ql.getAction();
    }

    EXPECT_NEAR(result, 0, 0.1);
    ql.printQ();
}
