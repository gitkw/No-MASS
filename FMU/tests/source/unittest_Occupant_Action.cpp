// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "tests/Gen.hpp"
#include "DataStore.hpp"
#include "Occupant_Action_Window.hpp"
#include "Utility.hpp"
#include "gtest/gtest.h"

class Test_Occupant_Action : public ::testing::Test {
 protected:
    Occupant_Action_Window aa;
    std::vector<double> activities;
    std::vector<int> availableActivities;
    virtual void SetUp();
};

void Test_Occupant_Action::SetUp() {
  Configuration::info.timeStepsPerHour = 12;
  Configuration::setStepCount(5);

  availableActivities.push_back(1);
  availableActivities.push_back(2);
  availableActivities.push_back(3);
  aa.setAvailableActivities(availableActivities);
  activities.push_back(0);
  activities.push_back(0);
  activities.push_back(1);
  activities.push_back(2);
  activities.push_back(0);
  activities.push_back(0);
  activities.push_back(1);
  activities.push_back(2);
  activities.push_back(3);
  activities.push_back(0);
}

TEST_F(Test_Occupant_Action, getFutureDurationOfAbsenceState) {
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(0);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 600);
  Configuration::setStepCount(1);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(2);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(3);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(4);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 600);
  Configuration::setStepCount(5);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(6);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(7);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(8);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(9);
  EXPECT_EQ(aa.getFutureDurationOfAbsenceState(activities), 300);
}


TEST_F(Test_Occupant_Action, getPreviousDurationOfAbsenceState) {
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(0);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(1);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(2);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 600);
  Configuration::setStepCount(3);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(4);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(5);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  Configuration::setStepCount(6);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 600);
  Configuration::setStepCount(7);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(8);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  Configuration::setStepCount(9);
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
}

TEST_F(Test_Occupant_Action, getCurrentDurationOfPresenceState) {
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(0);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(1);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(2);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(3);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 300);
  Configuration::setStepCount(4);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(5);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(6);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  Configuration::setStepCount(7);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 300);
  Configuration::setStepCount(8);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 600);
  Configuration::setStepCount(9);
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
}
