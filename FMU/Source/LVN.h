// Copyright 2016 Jacob Chapman

#ifndef LVN_H_
#define LVN_H_

#include <vector>
#include "LVN_Node.h"

class LVN {
 public:
  LVN();
  void setup();
  void postTimeStep();
  void setPowerForID(const double power, const int id);

 private:
  // double availablePower;
  // double basePower;

  LVN_Node rootNode;
  bool enabled;
};

#endif  // LVN_H_
