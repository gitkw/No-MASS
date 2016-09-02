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
  void setup(int agentid);
  double getPMV() const;
  double getPPD() const;

 private:
  int id;
  double clo;
  double metabolicRate;
  double ppd;
  double pmv;

  std::string stringIdMetabolicRate;
  std::string stringIdClo;
  std::string stringIdPpd;
  std::string stringIdPmv;
  std::string stringIdPmvAirTemp;
  std::string stringIdPmvAirHumid;
  std::string stringIdPmvMeanRadient;
  std::string stringIdPmvSetpoint;
};

#endif  // OCCUPANT_ACTION_HEAT_GAINS_H_
