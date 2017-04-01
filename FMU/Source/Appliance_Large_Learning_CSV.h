// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_LEARNING_CSV_H_
#define APPLIANCE_LARGE_LEARNING_CSV_H_

#include <vector>
#include <string>
#include "Appliance_Large_Learning.h"


/**
 * @brief Large appliances learning class
 * @details This will handle the appliance learning model for profile shifting
 */

class Appliance_Large_Learning_CSV : public Appliance_Large_Learning {

 public:
  Appliance_Large_Learning_CSV();


 protected:
  void setupModel();
  void calculateProfile();
 private:


};

#endif  // APPLIANCE_LARGE_LEARNING_CSV_H_
