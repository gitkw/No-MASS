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
  double indoorTemperature = 35;
  double outdoorTemperature = 10;
  double previousDuration = 5*12*10*60;
  bool rain = false;
  double timeStepLengthInMinutes = 5;
  mw.arrival(indoorTemperature, outdoorTemperature,
      previousDuration, rain, timeStepLengthInMinutes);

//  EXPECT_EQ(true, mw.getWindowState());

  mw.getDurationOpen();
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
