// Copyright 2015 Jacob Chapman

#ifndef STATE_H_
#define STATE_H_
#include <string>
#include <memory>
#include <unordered_map>
#include "Building_Zone.h"

class State {
 public:
  State();
  virtual ~State();
  virtual bool hasState(const int stateID) const;
  virtual State getState(const int stateID) const;

  void addState(State s);
  void setZonePtr(std::shared_ptr<Building_Zone> zoneptr);
  int getId() const;
  unsigned int numberOfSubStates() const;
  double getMetabolicRate() const;
  double getClo() const;
  bool isInActivity(const std::string & activity) const;
  std::shared_ptr<Building_Zone> getZonePtr() const;

 protected:
  int id;
  double metabolicRate;
  double clo;
  std::string activity;
  std::unordered_map<int, State> states;
  std::shared_ptr<Building_Zone> zone;
};

#endif  // STATE_H_
