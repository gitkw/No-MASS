// Copyright 2015 Jacob Chapman

#include <vector>
#include <string>
#include <iostream>
#include "DataStore.h"
#include "Model_HeatGains.h"
#include "Occupant_Action_Heat_Gains.h"

Occupant_Action_Heat_Gains::Occupant_Action_Heat_Gains() {}

void Occupant_Action_Heat_Gains::setup(int agentid) {
  this->id = agentid;
  std::string idAsString = std::to_string(id);
  stringIdMetabolicRate = "Occupant_Metabolic_Rate_" + idAsString;
  DataStore::addVariable(stringIdMetabolicRate);
  stringIdClo = "Occupant_clo_" + idAsString;
  DataStore::addVariable(stringIdClo);
  stringIdPpd = "Occupant_ppd_" + idAsString;
  DataStore::addVariable(stringIdPpd);
  stringIdPmv = "Occupant_pmv_" + idAsString;
  DataStore::addVariable(stringIdPmv);
  stringIdPmvAirTemp = "Occupant_PMV_airTemp" + idAsString;
  DataStore::addVariable(stringIdPmvAirTemp);
  stringIdPmvAirHumid = "Occupant_PMV_airHumid" + idAsString;
  DataStore::addVariable(stringIdPmvAirHumid);
  stringIdPmvMeanRadient = "Occupant_PMV_meanRadient" + idAsString;
  DataStore::addVariable(stringIdPmvMeanRadient);
  stringIdPmvSetpoint = "Occupant_PMV_setpoint" + idAsString;
  DataStore::addVariable(stringIdPmvSetpoint);
}

void Occupant_Action_Heat_Gains::prestep(double clo, double metabolicRate) {
    this->clo = clo;
    this->metabolicRate = metabolicRate;
}

void Occupant_Action_Heat_Gains::step(const Building_Zone& zone,
                                    const bool inZone) {
  ppd = 5;
  pmv = 0;
  result = 0;
  double airTemp = zone.getMeanAirTemperature();
  double airHumid = zone.getAirRelativeHumidity();
  double meanRadient = zone.getMeanRadiantTemperature();

  if (inZone) {
    Model_HeatGains h;
    /**
     * Calculates the Fanger pmv and sets the instance varibles related to results.
     * @param metabolicRate Metabolic Rate
     * @param partialWaterPressure partial water vapour kPa
     * @param meanRadiantTemperature mean radiant temperature C
     * @param externalWork external work
     * @param ta air temperature
     * @param clo Clothing value
     * @param airVelocity Air velocity
     */

    h.calculate(metabolicRate, airHumid, meanRadient, 0, airTemp, clo, 0.137);
    result = h.getAllHeatGains();
    ppd = h.getPpd();
    pmv = h.getPmv();
  }
  DataStore::addValue(stringIdMetabolicRate, metabolicRate);
  DataStore::addValue(stringIdClo, clo);
  DataStore::addValue(stringIdPpd, ppd);
  DataStore::addValue(stringIdPmv, pmv);
  DataStore::addValue(stringIdPmvAirTemp, airTemp);
  DataStore::addValue(stringIdPmvAirHumid, airHumid);
  DataStore::addValue(stringIdPmvMeanRadient, meanRadient);
  DataStore::addValue(stringIdPmvSetpoint, zone.getHeatingState());
}

double Occupant_Action_Heat_Gains::getPMV() const {
    return pmv;
}

double Occupant_Action_Heat_Gains::getPPD() const {
    return ppd;
}
