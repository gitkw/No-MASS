// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <fstream>
#include "Utility.h"
#include "gtest/gtest.h"

TEST(Utility, ran) {
  Utility::setSeed(1);

  EXPECT_NEAR(Utility::randomDouble(0, 1), 0.13640703, 0.001);
  EXPECT_EQ(Utility::randomInt(0, 1), 0);
  EXPECT_TRUE(Utility::tossACoin());
  EXPECT_TRUE(Utility::tossACoin());
  EXPECT_FALSE(Utility::tossACoin());
  EXPECT_NEAR(Utility::randomDouble(0, 1), 0.4707521, 0.001);
  //EXPECT_EQ(Utility::randomInt(0, 1000), 777);
}


TEST(Utility, Distribution) {
  Utility::setSeed(1);

  std::string file2 = "randomDistribution.csv";
  std::ofstream myfile2;
  myfile2.open(file2);
  for (int i = 0; i < 100000; i++) {
    myfile2 << Utility::randomDouble(0, 1);
    myfile2 << std::endl;
  }
  myfile2.close();
}


TEST(Utility, Days) {
  /*
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
*/
  int days = Utility::calculateNumberOfDays(1, 1, 1, 1);
  EXPECT_EQ(days, 1);
  days = Utility::calculateNumberOfDays(1, 1, 5, 1);
  EXPECT_EQ(days, 5);
  days = Utility::calculateNumberOfDays(1, 1, 5, 2);
  EXPECT_EQ(days, 36);
}
