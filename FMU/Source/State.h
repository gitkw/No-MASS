// Copyright 2015 Jacob Chapman

#ifndef STATE_H_
#define STATE_H_
#include <string>
#include <memory>
#include <vector>
#include "Building_Zone.h"

class State {
 public:
  State();
  State(int id, double metabolicRate, double clo, const std::string & activity);
  virtual ~State();
  virtual bool hasState(const int stateID) const;
  virtual State getState(const int stateID) const;

  void addState(State s);
  void setZonePtr(std::shared_ptr<Building_Zone> zoneptr);
  int getId() const;
  void setId(int id);
  void setActivity(const std::string & activity);
  void setMetabolicRate(double metabolicRate);
  void setClo(double clo);
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
  std::vector<State> states;
  std::shared_ptr<Building_Zone> zone;
};

#endif  // STATE_H_
