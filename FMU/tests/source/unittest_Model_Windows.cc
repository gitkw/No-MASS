// Copyright 2015 Jacob Chapman

#include <limits.h>
#include <string>
#include "Model_Windows.h"
#include "Utility.h"

#include "gtest/gtest.h"

class Test_Windows : public ::testing::Test {
 protected:
    Model_Windows mw;

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Windows::SetUp() {
}
void Test_Windows::AfterConfiguration() {
}

TEST_F(Test_Windows, Arrival) {
  Utility::setSeed(1);
  double indoorTemperature = 35;
  double outdoorTemperature = 10;
  double previousDuration = 5*12*10*60;
  bool rain = false;
  int timeStepLengthInMinutes = 5;

  for (int i =0; i < 1; i++) {
    mw.arrival(indoorTemperature, outdoorTemperature,
        previousDuration, rain, timeStepLengthInMinutes);
    EXPECT_TRUE(mw.getWindowState());
  }

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_NEAR(0, mw.getDurationOpen(), 0.01);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_NEAR(0, mw.getDurationOpen(), 0.01);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_NEAR(0, mw.getDurationOpen(), 0.01);
  EXPECT_FALSE(mw.getWindowState());

  int dur = mw.getDurationOpen();
  while (mw.getWindowState() == false || dur < 1000) {
    mw.arrival(indoorTemperature, outdoorTemperature,
        previousDuration, rain, timeStepLengthInMinutes);
    dur = mw.getDurationOpen();
  }

  EXPECT_NEAR(4754, mw.getDurationOpen(), 0.01);
  EXPECT_TRUE(mw.getWindowState());
}


TEST_F(Test_Windows, Inter) {
  Utility::setSeed(1);
  double indoorTemperature = 35;
  double outdoorTemperature = 10;
  double previousDuration = 5*12*10*60;
  bool rain = false;
  int timeStepLengthInMinutes = 5;
  int dur = 0;

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_TRUE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());

  EXPECT_EQ(0, dur);

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  dur = mw.getDurationOpen();
  EXPECT_EQ(0, dur);
  EXPECT_FALSE(mw.getWindowState());

  mw.intermediate(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  dur = mw.getDurationOpen();
  EXPECT_EQ(0, dur);
  EXPECT_FALSE(mw.getWindowState());

  mw.intermediate(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  dur = mw.getDurationOpen();
  EXPECT_EQ(0, dur);
  EXPECT_FALSE(mw.getWindowState());


  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_TRUE(mw.getWindowState());

  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_TRUE(mw.getWindowState());

  while (mw.getWindowState() == false || dur < 1000) {
    mw.arrival(indoorTemperature, outdoorTemperature,
        previousDuration, rain, timeStepLengthInMinutes);
    dur = mw.getDurationOpen();
  }

  EXPECT_TRUE(mw.getWindowState());

  EXPECT_EQ(4754, dur);
  int durCount = dur;
  while (dur > timeStepLengthInMinutes) {
    mw.intermediate(indoorTemperature, outdoorTemperature,
        previousDuration, rain, timeStepLengthInMinutes);
    dur = mw.getDurationOpen();
    durCount = durCount - timeStepLengthInMinutes;
    EXPECT_EQ(durCount, dur);
    EXPECT_TRUE(mw.getWindowState());
  }
  mw.intermediate(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);
  EXPECT_FALSE(mw.getWindowState());
}

TEST_F(Test_Windows, depart) {
  Utility::setSeed(1);
  double indoorTemperature = 35;
  double dailyMeanTemperature = 34;

  EXPECT_FALSE(mw.getWindowState());
  for (int i =0; i < 53; i++) {
    mw.departure(indoorTemperature, dailyMeanTemperature, 300, 0);
    EXPECT_FALSE(mw.getWindowState());
  }
  for (int i =0; i < 5; i++) {
    mw.departure(indoorTemperature, dailyMeanTemperature, 300, 0);
    EXPECT_TRUE(mw.getWindowState());
  }
}


TEST_F(Test_Windows, coefs) {

  mw.setDurationVars(1, 1, 1);
  mw.setArrivalVars(1, 1, 1, 1, 1);
  mw.setInterVars(1, 1, 1, 1, 1);
  mw.setDepartureVars(1, 1, 1, 1, 1, 1, 1, 1, 1);
  mw.arrival(1, 1, 1, 1, 1);
      EXPECT_EQ(1,mw.getAop());
      EXPECT_EQ(1,mw.getBopout());
      EXPECT_EQ(1,mw.getshapeop());
      EXPECT_EQ(1,mw.getA01arr());
      EXPECT_EQ(1,mw.getB01inarr());
      EXPECT_EQ(1,mw.getB01outarr());
      EXPECT_EQ(1,mw.getB01absprevarr());
      EXPECT_EQ(1,mw.getB01rnarr());
      EXPECT_EQ(1,mw.getA01int());
      EXPECT_EQ(1,mw.getB01inint());
      EXPECT_EQ(1,mw.getB01outint());
      EXPECT_EQ(1,mw.getB01presint());
      EXPECT_EQ(1,mw.getB01rnint());
      EXPECT_EQ(1,mw.getA01dep());
      EXPECT_EQ(1,mw.getB01outdep());
      EXPECT_EQ(1,mw.getB01absdep());
      EXPECT_EQ(1,mw.getB01gddep());
      EXPECT_EQ(1,mw.getA10dep());
      EXPECT_EQ(1,mw.getB10indep());
      EXPECT_EQ(1,mw.getB10outdep());
      EXPECT_EQ(1,mw.getB10absdep());
      EXPECT_EQ(1,mw.getB10gddep());

}
