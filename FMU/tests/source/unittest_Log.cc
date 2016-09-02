// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "Log.h"
#include "gtest/gtest.h"

TEST(Log, error) {
  LOG.reset();
  LOG << "hello";
  EXPECT_FALSE(LOG.getError());
  LOG.error();
  EXPECT_TRUE(LOG.getError());
  LOG.reset();
}
