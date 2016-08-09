// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_MODEL_APPLIANCE_PV_CSV_H_
#define FMU_SOURCE_MODEL_APPLIANCE_PV_CSV_H_

#include <vector>
class Model_Appliance_PV_CSV {
 public:
      Model_Appliance_PV_CSV();
      void parseConfiguration(const std::string filename);
      double supply(const int dayOfYear, const int minuteOfday);
 private:
   std::vector<std::vector<double>> profile;
};

#endif  // FMU_SOURCE_MODEL_APPLIANCE_PV_CSV_H_
