

#include <limits.h>

#include "Simulation.h"
#include "SimulationConfig.h"
#include "DataStore.h"

#include "gtest/gtest.h"

TEST(Simulation, HeatGainsOnly) {

  SimulationConfig::idfFileLocation = "tests/Files/in.idf";
  SimulationConfig::FmuLocation = "tests/Files";
  Simulation s;

  s.parseConfiguration(SimulationConfig::FmuLocation + "/SimulationConfig.xml");
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  s.setupSimulationModel();

  DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 21);
  DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 21);

  s.preTimeStep();
  s.timeStep();
  EXPECT_EQ(SimulationConfig::getStepCount(),0);
  int activity = DataStore::getValue("Agent_Activity_1");
  EXPECT_EQ(activity, 9);
  s.postTimeStep();
  s.postprocess();

  for(int i =1; i< 10000; i++){
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(),i);
    int activity = DataStore::getValue("Agent_Activity_1");

    s.postTimeStep();
    if(activity < 9){
      EXPECT_EQ(activity, 3);
      ASSERT_NEAR(DataStore::getValue("AgentGains1"), 99.4269209,0.001);
      ASSERT_NEAR(DataStore::getValue("Agent_Metabolic_Rate_1"), 116,0.001);
      ASSERT_NEAR(DataStore::getValue("Agent_clo_1"), 1,0.001);
      ASSERT_NEAR(DataStore::getValue("Agent_ppd_1"), 11,0.001);
      ASSERT_NEAR(DataStore::getValue("Agent_pmv_1"), 0,0.001);
    }

  }


  s.postprocess();

}


TEST(Simulation, HeatGainsWindowsOnly) {

  SimulationConfig::idfFileLocation = "tests/Files/in.idf";
  SimulationConfig::FmuLocation = "tests/Files";
  Simulation s;

  s.parseConfiguration(SimulationConfig::FmuLocation + "/SimulationConfig.xml");
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.windows = true;
  s.setupSimulationModel();


  DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 21);
  DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 21);
  DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 21);
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 18);
  DataStore::addValue("EnvironmentSiteRainStatus", 0);

  s.preTimeStep();
  s.timeStep();
  EXPECT_EQ(SimulationConfig::getStepCount(),0);
  int WindowState = DataStore::getValue("Block1:Zone1WindowState0");
  EXPECT_EQ(WindowState, 0);
  s.postTimeStep();
  s.postprocess();
  int i =1;
  for(; i < 100; i++){
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(),i);
    WindowState = DataStore::getValue("Block1:Zone1WindowState0");
    EXPECT_EQ(WindowState,0);
    s.postTimeStep();
  }

  DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 100);
  DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 100);
  DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 100);
  DataStore::addValue("EnvironmentSiteOutdoorAirDrybulbTemperature", 15);
  DataStore::addValue("EnvironmentSiteRainStatus", 0);

  for(i =100;; i++){
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(),i);
    WindowState = DataStore::getValue("Block1:Zone1WindowState0");
    int occs = DataStore::getValue("Block1:Zone1NumberOfOccupants");
    if(occs > 0){
      EXPECT_EQ(WindowState,1);
      break;
    }
    s.postTimeStep();
  }

  s.postprocess();

}
