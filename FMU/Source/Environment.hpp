// Copyright 2016 Jacob Chapman

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <deque>

 /**
  * @brief Stores environmental parameters
  * @details Stores environmental parameters such as extrenal weather conditions
  */

class Environment {
 public:
  static double dailyMeanTemperature;
  static void calculateDailyMeanTemperature();
  static double getDailyMeanTemperature();
  static double getEVG();
  static double getOutdoorAirDrybulbTemperature();

 private:
  static std::deque<double> outDoorTemperatures;
  Environment();
};

#endif  // ENVIRONMENT_H_
