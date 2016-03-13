// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "Utility.h"
#include "gtest/gtest.h"

TEST(Utility, ran) {
  Utility::setSeed(1);

  ASSERT_NEAR(Utility::randomDouble(0, 1), 0.601352605, 0.001);
  ASSERT_EQ(Utility::randomInt(0, 1), 1);
  ASSERT_TRUE(Utility::tossACoin());
  ASSERT_TRUE(Utility::tossACoin());
  ASSERT_FALSE(Utility::tossACoin());
  ASSERT_NEAR(Utility::randomDouble(0, 1), 0.5603899281, 0.001);
  ASSERT_EQ(Utility::randomInt(0, 1000), 810);
}
