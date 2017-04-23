// Copyright 2016 Jacob Chapman

#ifndef LVN_H_
#define LVN_H_

#include <vector>
#include "LVN_Node.hpp"

 /**
  * @brief Models the survival fuction for the activity model
  * @details The low voltage network used to calculate cable losses
  * \n Sancho-Tomás, A., Chapman, J., & Robinson, D. (2017). Extending No-MASS: Multi-Agent Stochastic Simulation for Demand Response of residential appliances. In Building Simulation 2017.
  */

class LVN {
 public:
  LVN();
  void setup();
  void postTimeStep();
  void setPowerForID(const double power, const int id);

 private:
  LVN_Node rootNode;
  bool enabled;
};

#endif  // LVN_H_
