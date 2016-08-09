// Copyright 2016 Jacob Chapman

#include <string>
#include "SimulationConfig.h"
#include "Utility.h"
#include "Appliance_PV.h"

Appliance_PV::Appliance_PV() {
}

void Appliance_PV::setup() {
  model.parseConfiguration(SimulationConfig::RunLocation + filename);
}

void Appliance_PV::preprocess() {
  int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                            SimulationConfig::info.startMonth,
                                            SimulationConfig::info.endDay,
                                            SimulationConfig::info.endMonth);
  int totoaltimesteps = days * 24 * SimulationConfig::info.timeStepsPerHour;

  for (int timeStep = 0; timeStep < totoaltimesteps; timeStep++) {
    int minute = (timeStep * SimulationConfig::lengthOfTimestep()) / 60;
    int minuteOfDay = minute % 1440;
    int hour = (timeStep * SimulationConfig::lengthOfTimestep()) / 3600;
    int day = hour / 24;

    int leninsec = SimulationConfig::lengthOfTimestep();
    int now = timeStep * leninsec;
    int end = timeStep * leninsec + leninsec;
    double p = 0;
    while (now < end) {
      p += model.supply(day, minuteOfDay);
      minuteOfDay += 1;
      if (minuteOfDay >= 1440) {
        minuteOfDay = 0;
        day +=1;
      }
      now += 60;
    }

    // watt hour per timeperhour
    power.push_back(0);
    supply.push_back(p);
  }
}

void Appliance_PV::setFileName(std::string filename) {
  this->filename = filename;
}
