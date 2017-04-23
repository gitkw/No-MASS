// Copyright 2016 Jacob Chapman

#include <deque>
#include "DataStore.hpp"
#include "Configuration.hpp"
#include "Environment.hpp"

double Environment::dailyMeanTemperature = 0;
std::deque<double> Environment::outDoorTemperatures;

Environment::Environment() {}

double Environment::getDailyMeanTemperature() {
  return dailyMeanTemperature;
}

void Environment::calculateDailyMeanTemperature() {
  if (Configuration::info.windows) {
    double outdoorTemperature = getOutdoorAirDrybulbTemperature();
    outDoorTemperatures.push_back(outdoorTemperature);
    if (outDoorTemperatures.size() >
          (Configuration::info.timeStepsPerHour * 24)) {
            outDoorTemperatures.pop_front();
    }
    dailyMeanTemperature = 0;
    for (const double temp : outDoorTemperatures) {
            dailyMeanTemperature += temp;
    }
    dailyMeanTemperature =
      dailyMeanTemperature /
        static_cast<double>(outDoorTemperatures.size());
  }
}


double Environment::getEVG() {
  return DataStore::getValueS("EnvironmentSiteExteriorHorizontalSkyIlluminance");
}


double Environment::getOutdoorAirDrybulbTemperature() {
  return DataStore::getValueS("EnvironmentSiteOutdoorAirDrybulbTemperature");
}
