// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "Gen.h"
#include "SimulationTime.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "gtest/gtest.h"

TEST(SimulationTime, Day) {
/*  SimulationConfig::info.timeStepsPerHour = 60;
  SimulationConfig::info.startDay = 0;
  SimulationConfig::info.startMonth = 0;
  SimulationConfig::info.endDay = 2;
  SimulationConfig::info.endMonth = 0;

  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);

  int totaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;
  EXPECT_EQ(days, 2);
  EXPECT_EQ(SimulationConfig::info.timeStepsPerHour, 60);
  EXPECT_EQ(totaltimesteps, 2880);
  SimulationTime::reset();
  SimulationTime::preprocess();

  EXPECT_EQ(SimulationTime::stepCount, -1);
  EXPECT_EQ(SimulationTime::day, 0);

  for (int i = 0; i < 1440; i++){
    SimulationTime::trackTime();
    EXPECT_EQ(SimulationTime::stepCount, i);
    EXPECT_EQ(SimulationTime::day, 0);
  }
  for (int i = 0; i < 1440; i++){
    SimulationTime::trackTime();
    EXPECT_EQ(SimulationTime::stepCount, i+1440);
    EXPECT_EQ(SimulationTime::day, 1);
  }
  for (int i = 0; i < 1440; i++){
    SimulationTime::trackTime();
    EXPECT_EQ(SimulationTime::stepCount, i+1440*2);
    EXPECT_EQ(SimulationTime::day, 2);
  }
  for (int i = 0; i < 1440; i++){
    SimulationTime::trackTime();
    EXPECT_EQ(SimulationTime::stepCount, i+1440*3);
    EXPECT_EQ(SimulationTime::day, 3);
  }
  */
}
