

#include <limits.h>
#include "Model_HeatGains.h"
#include "SimulationConfig.h"

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
    ASSERT_NEAR(heatgains, 69.5992,0.001);
    ASSERT_NEAR(ppd, 100,0.001);
    ASSERT_NEAR(pmv, -7.8653,0.001);


}
