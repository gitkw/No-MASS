// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <fstream>
#include "Utility.h"
#include "gtest/gtest.h"

TEST(Utility, ran) {
  Utility::setSeed(1);

  ASSERT_NEAR(Utility::randomDouble(0, 1), 0.13640703, 0.001);
  ASSERT_EQ(Utility::randomInt(0, 1), 0);
  ASSERT_TRUE(Utility::tossACoin());
  ASSERT_TRUE(Utility::tossACoin());
  ASSERT_FALSE(Utility::tossACoin());
  ASSERT_NEAR(Utility::randomDouble(0, 1), 0.4707521, 0.001);
  //ASSERT_EQ(Utility::randomInt(0, 1000), 777);
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
