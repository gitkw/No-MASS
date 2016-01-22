// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

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
}

TEST_F(Test_QLearning_HeatingSetPoints, OpenWindowDuringCooking) {
    ql.setId(1);
    ql.setup();

    ZoneStruct zs;
    zs.name = "Block1:Zone1";
    Zone z_Kitchen("", zs);

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
    ASSERT_NEAR(heating, 21, 0.1);

}
