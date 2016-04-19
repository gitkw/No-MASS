// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <vector>

#include "Gen.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"
#include "Utility.h"
#include "gtest/gtest.h"

class Test_Agent_Action : public ::testing::Test {
 protected:
    Agent_Action_Window aa;
    std::vector<double> activities;
    std::vector<int> availableActivities;
    virtual void SetUp();
};

void Test_Agent_Action::SetUp() {
  SimulationConfig::info.timeStepsPerHour = 12;
  SimulationConfig::stepCount = 5;

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

TEST_F(Test_Agent_Action, getFutureDurationOfPresenceState) {
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 0;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 1;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 2;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 600);
  SimulationConfig::stepCount = 3;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 300);
  SimulationConfig::stepCount = 4;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 5;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 6;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 900);
  SimulationConfig::stepCount = 7;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 600);
  SimulationConfig::stepCount = 8;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 300);
  SimulationConfig::stepCount = 9;
  EXPECT_EQ(aa.getFutureDurationOfPresenceState(activities), 0);
}


TEST_F(Test_Agent_Action, getPreviousDurationOfAbsenceState) {
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  SimulationConfig::stepCount = 0;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  SimulationConfig::stepCount = 1;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  SimulationConfig::stepCount = 2;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 600);
  SimulationConfig::stepCount = 3;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  SimulationConfig::stepCount = 4;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  SimulationConfig::stepCount = 5;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 300);
  SimulationConfig::stepCount = 6;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 600);
  SimulationConfig::stepCount = 7;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  SimulationConfig::stepCount = 8;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
  SimulationConfig::stepCount = 9;
  EXPECT_EQ(aa.getPreviousDurationOfAbsenceState(activities), 0);
}

TEST_F(Test_Agent_Action, getCurrentDurationOfPresenceState) {
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 0;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 1;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 2;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 3;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 300);
  SimulationConfig::stepCount = 4;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 5;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 6;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
  SimulationConfig::stepCount = 7;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 300);
  SimulationConfig::stepCount = 8;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 600);
  SimulationConfig::stepCount = 9;
  EXPECT_EQ(aa.getCurrentDurationOfPresenceState(activities), 0);
}
