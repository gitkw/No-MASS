

#include <limits.h>

#include "Simulation.h"
#include "SimulationConfig.h"
#include "DataStore.h"

#include "gtest/gtest.h"



TEST(Simulation, HeatGainsOnly) {

SimulationConfig::idfFileLocation = "tests/Files/in.idf";
SimulationConfig::FmuLocation = "tests/Files";
Simulation s;


s.preprocess();

SimulationConfig::info.windows = false;
SimulationConfig::info.shading = false;
SimulationConfig::info.lights = false;

DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 21);
DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 21);
DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 21);

s.preTimeStep();
s.timeStep();
EXPECT_EQ(SimulationConfig::getStepCount(),0);


s.postTimeStep();
/*
ASSERT_NEAR(DataStore::getValue("AgentGains1"), 69.5992,0.001);
ASSERT_NEAR(DataStore::getValue("Agent_Metabolic_Rate_1"), 69.5992,0.001);
ASSERT_NEAR(DataStore::getValue("Agent_clo_1"), 69.5992,0.001);
ASSERT_NEAR(DataStore::getValue("Agent_ppd_1"), 69.5992,0.001);
ASSERT_NEAR(DataStore::getValue("Agent_pmv_1"), 69.5992,0.001);
ASSERT_NEAR(DataStore::getValue("Agent_Fanger_Neutral_Temperature_1"), 69.5992,0.001);
*/
s.postprocess();


}


TEST(Simulation, HeatGainsWindowsOnly) {

SimulationConfig::idfFileLocation = "tests/Files/in.idf";
SimulationConfig::FmuLocation = "tests/Files";
Simulation s;

s.preprocess();

SimulationConfig::info.windows = true;
SimulationConfig::info.shading = false;
SimulationConfig::info.lights = false;

DataStore::addValue("Block1:Zone1ZoneMeanAirTemperature", 21);
DataStore::addValue("Block1:Zone1ZoneAirRelativeHumidity", 21);
DataStore::addValue("Block1:Zone1ZoneMeanRadiantTemperature", 21);

s.preTimeStep();
s.timeStep();
EXPECT_EQ(SimulationConfig::getStepCount(),0);


s.postTimeStep();
s.postprocess();


}
