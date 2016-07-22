// Copyright 2016 Jacob Chapman

#include "Appliance.h"

Appliance::Appliance() {
}


double Appliance::powerAt(const int timestep) const {
  return power[timestep];
}
