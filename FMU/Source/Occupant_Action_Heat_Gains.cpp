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
  idAsString = std::to_string(id);
  DataStore::addVariable("Occupant_Metabolic_Rate_" + idAsString);
  DataStore::addVariable("Occupant_clo_" + idAsString);
  DataStore::addVariable("Occupant_ppd_" + idAsString);
  DataStore::addVariable("Occupant_pmv_" + idAsString);
  DataStore::addVariable("Occupant_Fanger_Neutral_Temperature_" + idAsString);
  DataStore::addVariable("Occupant_PMV_airTemp" + idAsString);
  DataStore::addVariable("Occupant_PMV_airHumid" + idAsString);
  DataStore::addVariable("Occupant_PMV_meanRadient" + idAsString);
  DataStore::addVariable("Occupant_PMV_setpoint" + idAsString);
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
/*
if(zone.getId() == 40){
    std::cout << "#### metabolicRate: " << metabolicRate << std::endl;
    std::cout << "#### metabolicRate: " << metabolicRate / 58.15 << std::endl;
    std::cout << "#### airHumid: " << airHumid << std::endl;
    std::cout << "#### meanRadient: " << meanRadient << std::endl;
    std::cout << "#### airTemp: " << airTemp << std::endl;
    std::cout << "#### clo: " << clo << std::endl;
    std::cout << "#### ppd: " << ppd << std::endl;
    std::cout << "#### pmv: " << pmv << std::endl;
    std::cout << "#### HeatGains: " << result << std::endl;
  }
*/
  }
  std::string name;
  name = "Occupant_Metabolic_Rate_" + idAsString;
  DataStore::addValue(name.c_str(), metabolicRate);
  name = "Occupant_clo_" + idAsString;
  DataStore::addValue(name.c_str(), clo);
  name = "Occupant_ppd_" + idAsString;
  DataStore::addValue(name.c_str(), ppd);
  name = "Occupant_pmv_" + idAsString;
  DataStore::addValue(name.c_str(), pmv);
  name = "Occupant_PMV_airTemp" + idAsString;
  DataStore::addValue(name.c_str(), airTemp);
  name = "Occupant_PMV_airHumid" + idAsString;
  DataStore::addValue(name.c_str(), airHumid);
  name = "Occupant_PMV_meanRadient" + idAsString;
  DataStore::addValue(name.c_str(), meanRadient);
  name = "Occupant_PMV_setpoint" + idAsString;
  DataStore::addValue(name.c_str(), zone.getHeatingState());
}

double Occupant_Action_Heat_Gains::getPMV() const {
    return pmv;
}

double Occupant_Action_Heat_Gains::getPPD() const {
    return ppd;
}
