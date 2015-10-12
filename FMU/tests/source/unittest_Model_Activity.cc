

#include <limits.h>
#include "Model_Activity.h"
#include "SimulationConfig.h"

#include "gtest/gtest.h"


TEST(Activity, Dissagregate) {
    SimulationConfig::agents.clear();

    SimulationConfig::parseConfiguration("tests/Files/SimulationConfig.xml");
    SimulationConfig::info.timeSteps = 105182;
    SimulationConfig::info.timeStepsPerHour = 6;
    Model_Activity ma;
    std::vector<double> activities = ma.getAgentActivities(0);
    EXPECT_EQ(activities.at(0), 0);
    EXPECT_EQ(activities.at(1000), 0);
    EXPECT_EQ(activities.at(2000), 2);
    EXPECT_EQ(activities.at(3000), 1);
    EXPECT_EQ(activities.at(4000), 1);
    EXPECT_EQ(activities.at(5000), 9);
    EXPECT_EQ(activities.at(6000), 9);

}

TEST(Activity, multinominal) {
    SimulationConfig::agents.clear();

    SimulationConfig::parseConfiguration("tests/Files/SimulationConfig1.xml");
    SimulationConfig::info.timeSteps = 105182;
    SimulationConfig::info.timeStepsPerHour = 6;
    SimulationConfig::info.startDayOfWeek = 1;
    Model_Activity ma;
    std::vector<double> activities = ma.getAgentActivities(0);
    EXPECT_EQ(activities.at(0), 9);
    EXPECT_EQ(activities.at(1000), 9);
    EXPECT_EQ(activities.at(2000), 9);
    EXPECT_EQ(activities.at(3000), 9);
    EXPECT_EQ(activities.at(4000), 8);
    EXPECT_EQ(activities.at(5000), 9);
    EXPECT_EQ(activities.at(6000), 9);

}
/*
TEST(Appliance, Ownership) {

  Appliance a;
  ASSERT_NEAR(0.932138, a.ownership(),0.001);
  EXPECT_GT(a.ownership(), 0);
  EXPECT_GT(1, a.ownership());

}



TEST(Appliance, switchon) {

  Appliance a;
  a.setAppliance(2);
  //EXPECT_FALSE(a.onAt(1));
  ASSERT_NEAR(0.0308,a.getMeanFraction(),0.001);


}
*/
