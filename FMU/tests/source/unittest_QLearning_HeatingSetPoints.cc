// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>
#include <fstream>
#include <cmath>

#include "DataStore.h"
#include "QLearning_Occupant.h"

#include "Building_Zone.h"
#include "SimulationConfig.h"

#include "SimulationTime.h"
#include "QLearning.h"
#include "gtest/gtest.h"

class Test_QLearning_HeatingSetPoints : public ::testing::Test {
 protected:
    QLearning_Occupant ql;
    int dataVal = -1;
    virtual void SetUp();
};

void Test_QLearning_HeatingSetPoints::SetUp() {
  ql.setStates(288);
  ql.setActions(20);
  SimulationConfig::info.learnep = 0.8;
  SimulationConfig::outputRegexs = {".*"};
  dataVal = DataStore::addVariable("Block1:Zone1ZoneMeanAirTemperature");
  SimulationTime::hour = 1;
  SimulationTime::month = 1;
  SimulationTime::hourOfDay = 1;
}

TEST_F(Test_QLearning_HeatingSetPoints, learn) {
    ql.setId(1);
    ql.setup();

    ZoneStruct zs;
    zs.name = "Block1:Zone1";
    zs.id = 1;
    Building_Zone z_Kitchen;
    z_Kitchen.setName(zs.name);
    z_Kitchen.setActive(true);
    z_Kitchen.setup(zs);

    DataStore::addValue(dataVal, 1);

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
        ql.setState(0);
      heating = ql.learn();
      DataStore::addValue(dataVal, heating);
    }
    // ql.printQ();

    ql.setEpsilon(0.0);
   // heating = ql.learn();
  //  EXPECT_NEAR(heating, 21, 0.1);
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

      reward = reward * (1-x) - 2 * x;
      reward = - std::abs(reward);
      if (reward > 0.5) {
        reward = 1;
      }

      ql.updateQ(previous_state, action, reward, state);
    }

    ql.printQ();
}
