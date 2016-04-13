// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>
#include <fstream>

#include "DataStore.h"
#include "QLearning_HeatingSetPoints.h"
#include "QLearning_HeatingRate.h"
#include "QLearning.h"
#include "gtest/gtest.h"

class Test_QLearning_HeatingSetPoints : public ::testing::Test {
 protected:

    QLearning_HeatingSetPoints ql;
    virtual void SetUp();
};

void Test_QLearning_HeatingSetPoints::SetUp() {
  ql.setStates(288);
  SimulationConfig::info.learnep = 0.8;
  DataStore::addVariable("hour");
  DataStore::addVariable("month");
  DataStore::addVariable("hourOfDay");
  DataStore::addValue("hour", 1);
  DataStore::addValue("month", 1);
  DataStore::addValue("hourOfDay", 1);
}

TEST_F(Test_QLearning_HeatingSetPoints, learn) {
    ql.setId(1);
    ql.setup();

    ZoneStruct zs;
    zs.name = "Block1:Zone1";
    zs.id = 1;
    Building_Zone z_Kitchen("", zs);

    DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 1);

    double reward = -0.1;
    double heating;
    for (int i =0; i < 100000; i++) {
      reward = -0.01;
      if (static_cast<int>(z_Kitchen.getMeanAirTemperature()) == 20) {
        reward = 0.5;
      }
      if (static_cast<int>(z_Kitchen.getMeanAirTemperature()) == 22) {
        reward = 0.5;
      }
      if (static_cast<int>(z_Kitchen.getMeanAirTemperature()) == 21) {
        reward = 0.9;
      }
      ql.setReward(reward);
      heating = ql.learn(z_Kitchen);
      DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature",
                          heating);
    }
    // ql.printQ();

    ql.setEpsilon(0.0);
    heating = ql.learn(z_Kitchen);
  //  ASSERT_NEAR(heating, 21, 0.1);
}


TEST_F(Test_QLearning_HeatingSetPoints, Learn1) {
  ql.setId(1);
  ql.setup();
  std::ifstream in_file;
  in_file.open("../tests/Files/Data3.csv");

  while (in_file.good()) {
      std::string a;
      in_file >> a;
      std::stringstream ss(a);
      std::string item;
      std::getline(ss, item, ',');
      if (item == "") break;
      double previous_state = std::stod(item);
      std::getline(ss, item, ',');
      double state = std::stod(item);
      std::getline(ss, item, ',');
      double action = std::stod(item);
      std::getline(ss, item, ',');
      double reward = std::stod(item);
      int x = 0;
      if (previous_state > state) {
        x = (24 - previous_state) + state;
      } else {
        x = state - previous_state;
      }
      if (x > 2 && action > 10) {
        x = 1;
      } else {
        x = 0;
      }
/*
      if (reward > 0.0) {
        reward = reward * 6;
      }
*/
      reward = - std::abs((reward * (1-x)) - (2 * x));
      if (reward > 0.5) {
        reward = 1;
      }
/*
      std::cout << previous_state << " "
                << state << " "
                << action << " "
                << reward << " " << std::endl;
*/
      ql.updateQ(previous_state, action, reward, state);

    }

    ql.printQ();
}
