// Copyright 2015 Jacob Chapman

#include "State_Out.h"

State_Out::State_Out() {
  id = 9;  // out
  metabolicRate = 0;
  clo = 1;
  activity = "Out";
}

bool State_Out::hasZone() const {
    return false;
}
