// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "Model_HeatGains.hpp"
#include "Configuration.hpp"
#include "tests/Gen.hpp"

#include "gtest/gtest.h"

TEST(HeatGains, test) {
    Model_HeatGains h;
    double airTemp = 21;
    double airHumid = 15;
    double meanRadient = 22;
    double metabolicRate = 22;
    double clo = 1;
    h.calculate(metabolicRate, airHumid, meanRadient, 0, airTemp, clo, 0);
    double heatgains = h.getAllHeatGains();
    double ppd = h.getPpd();
    double pmv = h.getPmv();
    EXPECT_NEAR(heatgains, 69.678744, 0.001);
    EXPECT_NEAR(ppd, 100, 0.001);
    EXPECT_NEAR(pmv, -7.8784461, 0.001);
}


TEST(HeatGains, test2) {
  Model_HeatGains h;
  double airTemp = 25;
  double airHumid = 50;
  double meanRadient = 25;
  double metabolicRate = 58.15 * 1.1;//63.965;
  double clo = 0.5;
  double work = 0;
  double air = 0.1;
  h.calculate(metabolicRate, airHumid, meanRadient, work, airTemp, clo, air);
  double heatgains = h.getAllHeatGains();
  double ppd = h.getPpd();
  double pmv = h.getPmv();
  //  EXPECT_NEAR(heatgains, 69.5992,0.001);
  //  EXPECT_NEAR(ppd, 100,0.001);
  EXPECT_NEAR(pmv, -0.126434037, 0.001);
}


TEST(HeatGains, test3) {
  Model_HeatGains h;
  double airTemp = 26;
  double airHumid = 50;
  double meanRadient = 25;
  double metabolicRate = 58.15 * 1.1;//63.965;
  double clo = 0.5;
  double work = 0;
  double air = 0.1;
  h.calculate(metabolicRate, airHumid, meanRadient, work, airTemp, clo, air);
  double heatgains = h.getAllHeatGains();
  double ppd = h.getPpd();
  double pmv = h.getPmv();
  //  EXPECT_NEAR(heatgains, 69.5992,0.001);
  //  EXPECT_NEAR(ppd, 100,0.001);
  //EXPECT_NEAR(pmv, -0.06860287, 0.001);
  EXPECT_NEAR(pmv, 0.046210610, 0.001);
}

TEST(HeatGains, test4) {
  Model_HeatGains h;
  double airTemp = 29;
  double airHumid = 50;
  double meanRadient = 25;
  double metabolicRate = 58.15 * 1.1;//63.965;
  double clo = 0.5;
  double work = 0;
  double air = 0.1;
  h.calculate(metabolicRate, airHumid, meanRadient, work, airTemp, clo, air);
  double heatgains = h.getAllHeatGains();
  double ppd = h.getPpd();
  double pmv = h.getPmv();
  //  EXPECT_NEAR(heatgains, 69.5992,0.001);
  //  EXPECT_NEAR(ppd, 100,0.001);
  //EXPECT_NEAR(pmv, -0.06860287, 0.001);
  EXPECT_NEAR(pmv, 0.57141005349, 0.001);
}

TEST(HeatGains, test5) {
  Model_HeatGains h;
  double airTemp = 13;
  double airHumid = 50;
  double meanRadient = 25;
  double metabolicRate = 58.15 * 1.1;//63.965;
  double clo = 0.5;
  double work = 0;
  double air = 0.1;
  h.calculate(metabolicRate, airHumid, meanRadient, work, airTemp, clo, air);
  double heatgains = h.getAllHeatGains();
  double ppd = h.getPpd();
  double pmv = h.getPmv();
  //  EXPECT_NEAR(heatgains, 69.5992,0.001);
  //  EXPECT_NEAR(ppd, 100,0.001);
  //EXPECT_NEAR(pmv, -0.06860287, 0.001);
  EXPECT_NEAR(pmv, -2.5655824, 0.001);
}
