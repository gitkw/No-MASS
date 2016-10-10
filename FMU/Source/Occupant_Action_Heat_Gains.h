// Copyright 2016 Jacob Chapman

#ifndef OCCUPANT_ACTION_HEAT_GAINS_H_
#define OCCUPANT_ACTION_HEAT_GAINS_H_

#include <string>

#include "Occupant_Action.h"

class Occupant_Action_Heat_Gains : public Occupant_Action {
 public:
  Occupant_Action_Heat_Gains();
  void prestep(double clo, double metabolicRate);
  void step(const Building_Zone& zone, const bool inZone);
  void setup(int buildingID, int agentid);
  double getPMV() const;
  double getPPD() const;

 private:
  int id;
  int buildingID;
  double clo;
  double metabolicRate;
  double ppd;
  double pmv;

  int datastoreIdMetabolicRate;
  int datastoreIdClo;
  int datastoreIdPpd;
  int datastoreIdPmv;
  int datastoreIdPmvAirTemp;
  int datastoreIdPmvAirHumid;
  int datastoreIdPmvMeanRadient;
  int datastoreIdPmvSetpoint;
};

#endif  // OCCUPANT_ACTION_HEAT_GAINS_H_
