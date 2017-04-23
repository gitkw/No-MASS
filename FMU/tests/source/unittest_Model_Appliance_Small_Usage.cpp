// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <fstream>
#include "Model_Appliance_Small_Usage.hpp"
#include "tests/Gen.hpp"
#include "Utility.hpp"
#include "gtest/gtest.h"


class Appliance_Small_Usage : public ::testing::Test {
 protected:

    Model_Appliance_Small_Usage smallApp;
    virtual void SetUp();
};

void Appliance_Small_Usage::SetUp() {
  Utility::setSeed(0);
  smallApp.setFolderLocation(testFiles + "/SmallAppliances/");
  smallApp.readWeibullParameters("weibull_parameters_audiovisual.csv");
  smallApp.readStateProbabilities("state_probability_audiovisual.csv");
  smallApp.readFractions("mean_fractional_audiovisual.csv");
  smallApp.readSumRatedPowers("sum_rated_av.txt");
  smallApp.setRatedPowerAt(0);


}

TEST_F(Appliance_Small_Usage, weibull) {
  Utility::setSeed(0);
  double loc = 8.923806460231564;
  double shape = 0.7429984677226016;
  double scale = 10.735999774030896;

  EXPECT_NEAR(smallApp.weibullInvCdf(loc, shape, scale), 98.7442, 0.001);
}

TEST_F(Appliance_Small_Usage, weibullFromFile) {
  Utility::setSeed(0);
  EXPECT_NEAR(smallApp.durationAtState(0), 98.7442, 0.001);
}

TEST_F(Appliance_Small_Usage, stateFromFile) {
  Utility::setSeed(0);
  EXPECT_EQ(smallApp.calculateStateAtTenMin(0), 10);
}

TEST_F(Appliance_Small_Usage, All) {
  Utility::setSeed(0);
  int duration = 0;
  int timestep = 10;
  int days = 1;
  int state = 0;
  double consumption[144] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  for(int i = 0; i < days; i++){
    for(int j = 0; j < 144; j++){
      if (duration <= 0) {
        state = smallApp.calculateStateAtTenMin(j);
        duration = smallApp.durationAtState(state);
      }
      consumption[j] += smallApp.getFractionalPowerAtState(state);
      duration = duration - timestep;
    }
  }

  std::ofstream myfile2;
  myfile2.open("SmallAppliancesDemand.csv");
  for (int timeStep = 0; timeStep < 144; timeStep++) {
    myfile2 << (consumption[timeStep]) / days;
    myfile2 << std::endl;
  }
  myfile2.close();


}
