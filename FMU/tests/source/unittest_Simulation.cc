// Copyright 2015 Jacob Chapman

#include <limits.h>

#include "Gen.h"
#include "Simulation.h"
#include "SimulationConfig.h"
#include "SimulationTime.h"
#include "DataStore.h"

#include "gtest/gtest.h"

TEST(Simulation, HeatGainsOnly) {
  DataStore::clear();
  SimulationConfig::reset();
  SimulationConfig::RunLocation  = testFiles;
  Simulation s;
  s.parseConfiguration(SimulationConfig::RunLocation + "/SimulationConfig.xml");
  SimulationConfig::info.windows = false;
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationTime::preprocess();
  s.setupSimulationModel();

  s.preTimeStep();
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);
  s.timeStep();
  EXPECT_EQ(SimulationConfig::getStepCount(), 0);
  int activity = DataStore::getValueS("Building0_Occupant1_Activity");
  EXPECT_EQ(activity, 9);
  s.postTimeStep();

  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 21);
  DataStore::addVariable("Block1:Zone1ZoneMeanAirTemperature");
  DataStore::addVariable("Block1:Zone1ZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:Zone1ZoneMeanRadiantTemperature");
  DataStore::addValueS("Block1:Zone1ZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:Zone1ZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:Zone1ZoneMeanRadiantTemperature", 21);
  for (int i =1; i< 10000; i++) {
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(), i);
    int activity = DataStore::getValueS("Building0_Occupant1_Activity");

    s.postTimeStep();
    if (activity < 9) {
      EXPECT_EQ(activity, 3);
      EXPECT_NEAR(
        DataStore::getValueS("Building0_Occupant1_HeatGains"), 77.04634, 0.001);
      EXPECT_NEAR(
        DataStore::getValueS("Building0_Occupant1_Metabolic_Rate"), 70, 0.001);
      EXPECT_NEAR(DataStore::getValueS("Building0_Occupant1_CLO"), 1, 0.001);
      EXPECT_NEAR(
        DataStore::getValueS("Building0_Occupant1_PPD"), 7.83577821968, 0.001);
      EXPECT_NEAR(
        DataStore::getValueS("Building0_Occupant1_PMV"), -0.3690821420, 0.001);
    }
  }
  s.postprocess();
}


TEST(Simulation, HeatGainsWindowsOnly) {
  DataStore::clear();
  SimulationConfig::reset();
  SimulationConfig::RunLocation  = testFiles;
  Simulation s;

  s.parseConfiguration(SimulationConfig::RunLocation + "/SimulationConfig.xml");
  SimulationConfig::info.shading = false;
  SimulationConfig::info.lights = false;
  SimulationConfig::info.windows = true;
  SimulationTime::preprocess();
  s.setupSimulationModel();

  DataStore::addVariable("Block1:Zone1ZoneMeanAirTemperature");
  DataStore::addVariable("Block1:Zone1ZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:Zone1ZoneMeanRadiantTemperature");
  DataStore::addValueS("Block1:Zone1ZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:Zone1ZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:Zone1ZoneMeanRadiantTemperature", 21);
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("EnvironmentSiteRainStatus");
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 18);
  DataStore::addValueS("EnvironmentSiteRainStatus", 0);

  s.preTimeStep();
  s.timeStep();
  EXPECT_EQ(SimulationConfig::getStepCount(), 0.0);
  int WindowState = DataStore::getValueS("Block1:Zone1WindowState0");
  EXPECT_EQ(WindowState, 0);
  s.postTimeStep();

  DataStore::addVariable("Block1:Zone1ZoneMeanAirTemperature");
  DataStore::addVariable("Block1:Zone1ZoneAirRelativeHumidity");
  DataStore::addVariable("Block1:Zone1ZoneMeanRadiantTemperature");
  DataStore::addVariable("EnvironmentSiteOutdoorAirDrybulbTemperature");
  DataStore::addVariable("EnvironmentSiteRainStatus");
  DataStore::addValueS("Block1:Zone1ZoneMeanAirTemperature", 21);
  DataStore::addValueS("Block1:Zone1ZoneAirRelativeHumidity", 21);
  DataStore::addValueS("Block1:Zone1ZoneMeanRadiantTemperature", 21);
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 18);
  DataStore::addValueS("EnvironmentSiteRainStatus", 0);

  int i = 1;
  for ( ; i < 100; i++) {
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(), i);
    WindowState = DataStore::getValueS("Block1:Zone1WindowState0");
    // EXPECT_EQ(WindowState, 0);
    s.postTimeStep();
  }

  DataStore::addValueS("Block1:Zone1ZoneMeanAirTemperature", 28);
  DataStore::addValueS("Block1:Zone1ZoneAirRelativeHumidity", 100);
  DataStore::addValueS("Block1:Zone1ZoneMeanRadiantTemperature", 100);
  DataStore::addValueS("EnvironmentSiteOutdoorAirDrybulbTemperature", 15);
  DataStore::addValueS("EnvironmentSiteRainStatus", 0);

  for (i =100;; i++) {
    s.preTimeStep();
    s.timeStep();
    EXPECT_EQ(SimulationConfig::getStepCount(), i);
    WindowState = DataStore::getValueS("Block1:Zone1WindowState0");
    int occs = DataStore::getValueS("Block1:Zone1NumberOfOccupants");
    if (occs > 0) {
      //  EXPECT_EQ(WindowState, 1);
      break;
    }
    s.postTimeStep();
  }

  s.postprocess();
}
