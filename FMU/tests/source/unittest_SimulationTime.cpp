// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "tests/Gen.hpp"
#include "SimulationTime.hpp"
#include "Configuration.hpp"
#include "Utility.hpp"
#include "gtest/gtest.h"

TEST(SimulationTime, Day) {
/*  Configuration::info.timeStepsPerHour = 60;
  Configuration::info.startDay = 0;
  Configuration::info.startMonth = 0;
  Configuration::info.endDay = 2;
  Configuration::info.endMonth = 0;

  int days = Utility::calculateNumberOfDays(Configuration::info.startDay,
                                            Configuration::info.startMonth,
                                            Configuration::info.endDay,
                                            Configuration::info.endMonth);

  int totaltimesteps = days * 24 * Configuration::info.timeStepsPerHour;
  EXPECT_EQ(days, 2);
  EXPECT_EQ(Configuration::info.timeStepsPerHour, 60);
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
