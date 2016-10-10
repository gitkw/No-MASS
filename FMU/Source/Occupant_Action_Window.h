// Copyright 2016 Jacob Chapman

#ifndef OCCUPANT_ACTION_WINDOW_H_
#define OCCUPANT_ACTION_WINDOW_H_

#include "Occupant_Action.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Occupant_Action_Window : public Occupant_Action {
 public:
  Occupant_Action_Window();
  void setup(int windowID, int id);
  void setDailyMeanTemperature(double dailyMeanTemperature);
  void saveResult();
  int durationOpen() const;

 protected:
  Model_Windows m_window;
  int variableNameWindowDesire;
  double dailyMeanTemperature;

 private:
};

#endif  // OCCUPANT_ACTION_WINDOW_H_
