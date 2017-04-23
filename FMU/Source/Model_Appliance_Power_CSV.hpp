// Copyright 2016 Jacob Chapman

#ifndef MODEL_APPLIANCE_POWER_CSV_H_
#define MODEL_APPLIANCE_POWER_CSV_H_

#include <vector>
#include <string>

 /**
  * @brief Appliance modeled from a CSV file
  * @details Models appliance useage at each timestep from a CSV file
  */

class Model_Appliance_Power_CSV {
 public:
  Model_Appliance_Power_CSV();
  void parseConfiguration(const std::string & filename);
  double power(const int dayOfYear, const int minuteOfday);
 private:
  std::vector<std::vector<double>> profile;
};

#endif  // MODEL_APPLIANCE_POWER_CSV_H_
