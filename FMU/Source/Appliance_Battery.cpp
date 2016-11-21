// Copyright 2016 Jacob Chapman

#include <vector>
#include <string>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"
#include "Appliance_Battery.h"

Appliance_Battery::Appliance_Battery() {
  stateOfCharge = 0;
}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 */
void Appliance_Battery::setup() {
  std::string buildingString = "Building" + std::to_string(buildingID);
  std::string s_fullname = buildingString + "_Appliance";
  s_fullname = s_fullname + std::to_string(id) + "_";
  qLearning.setEpsilon(epsilon);
  qLearning.setAlpha(alpha);
  qLearning.setGamma(gamma);
  qLearning.setUpdate(update);
  qLearning.setFilename(s_fullname);
  qLearning.setStates(1440 / 60);
  qLearning.setActions(2);
  qLearning.setId(id);
  qLearning.setup();
  batteryPower = 0;
  MaxPower = 0;
  binShortage = 0;
  stateOfCharge = 0;
  datastoreIDstateOfCharge = DataStore::addVariable(s_fullname + "stateOfCharge");
}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 * @details Sets the large appliance configuration file and gives the model
 * the id of the appliance in the file
 */
void Appliance_Battery::setupModel() {}

/**
 * @brief The timestep call of the battery appliance
 * @details
 */
void Appliance_Battery::step() {

  double leninsec = SimulationConfig::lengthOfTimestep();
  double delta_t = leninsec / 60.0 / 60.0;
  get_new_SOC_charge(delta_t, recieved);

  power = 0;
  if(stateOfCharge < 100) {
    power = get_charge_delta();
  }

  int hourOfTheDay = calculateHourOfDay();
  if (hourOfTheDay != previousHourOfDay) {
    previousHourOfDay = hourOfTheDay;
    binShortage = sumShort - sumSupply;
    if (binShortage <= 0 ) {
      binShortage = 0.0000001;
    }
    if (binShortage > mostShortage) {
      mostShortage = binShortage;
    }
    double reward = 100.0 / mostShortage * binShortage;
    reward = reward / 100;
    if(action == 0 && reward > 0.7){
      reward = -reward;
    } else if (action == 1 && reward < 0.7){
      reward = -reward;
    }
    qLearning.setReward(reward);
    qLearning.setState(hourOfTheDay);
    qLearning.learn();
    action = qLearning.getAction();
    sumSupply = 0.0000001;
    sumShort = 0.0000001;
  }

  supply = 0.0;
  if (action) {
    if (powerShortage > 0 && stateOfCharge > 0) {
      supply = get_new_SOC_discharge(delta_t, powerShortage);
      power = 0.0;
      sumSupply += supply;
    }
  }
  if (powerShortage > 0){
    sumShort += powerShortage;
  }
  recieved = 0;
  DataStore::addValue(datastoreIDstateOfCharge, stateOfCharge);
}

void Appliance_Battery::clear() {
  local = false;
  global = false;
  power = 0.0;
  supply = 0.0;
  supplyCost = 0.0;
  recievedCost = 0.0;

}

/**
 * @brief Appliance prostprocess, calls Q-Learning save data
 */
void Appliance_Battery::postprocess() {
  qLearning.printQ();
}

void Appliance_Battery::setPowerShortage(double power) {
  this->powerShortage = power;
}

void Appliance_Battery::setEpsilon(double epsilon) {
  this->epsilon = epsilon;
}

void Appliance_Battery::setAlpha(double alpha) {
  this->alpha = alpha;
}

void Appliance_Battery::setGamma(double gamma) {
  this->gamma = gamma;
}

void Appliance_Battery::setUpdate(bool update) {
  this->update = update;
}

void Appliance_Battery::AddCost(double cost) {
  this->cost += cost;
}
/*
double Appliance_Battery::Voc(double SOC) {
    double a3 = 8.752e-5;
    double a2 = -0.022;
    double a1 = 2.668;
    double a0 = 254.7;
    return a0 + a1*SOC +a2*SOC*SOC + a3*SOC*SOC*SOC;
}

double Appliance_Battery::Rbatt(double SOC) {
    double b4 = -2.7923e-8;
    double b3 = 7.0533e-6;
    double b2 = -6.4879e-4;
    double b1 = 0.0239;
    double b0 = 0.0418;
    return b0 + b1*SOC + b2*SOC*SOC + b3*SOC*SOC*SOC + b4*SOC*SOC*SOC*SOC;
}

double Appliance_Battery::Vter_disch(double SOC) {
    return Voc(SOC)-2*Rbatt(SOC);
}

double Appliance_Battery::Vter_ch(double SOC) {
    return 2*Voc(SOC) - Vter_disch(SOC);
}

double Appliance_Battery::P_ch(double SOC) {
    return Vter_ch(SOC)*I;
}

double Appliance_Battery::P_dis(double SOC) {
    return Vter_disch(SOC)*I;
}
*/
double Appliance_Battery::get_charge_delta() {

    double P_request = chargeRate;
    double charge_left = capacity - energy_calc(stateOfCharge, capacity);

    if (charge_left < P_request){
      P_request = charge_left;
    }
    return P_request;
}

void Appliance_Battery::get_new_SOC_charge(double delta_t, double P_request) {
    double delta_E = delta_t * P_request * efficiency; // variation of energy that you will be able to charge in the duration of a time step
    energy = energy_calc(stateOfCharge, capacity) + delta_E; // new capacity iswhat you had plus what you charge
    stateOfCharge = energy / capacity * 100.0 ; // SOC is fraction of capacities
    if (stateOfCharge > 100){
      stateOfCharge = 100;
    }
}

double Appliance_Battery::get_new_SOC_discharge(double delta_t, double P_request) {
    if (P_request > dischargeRate){
      P_request = dischargeRate;
    }

    delta_E = delta_t * P_request * efficiency; // variation of energy that you will get in the duration of a time step
    energy = energy_calc(stateOfCharge, capacity) - delta_E; // new capacity, after removing what you discharged
    stateOfCharge = energy / capacity * 100.0;
    if (stateOfCharge < 0){
      stateOfCharge = 0;
    }
    return P_request;
}

double Appliance_Battery::energy_calc(double SOC, double capacity) {
    return SOC * capacity / 100; // this is the capacity at the correspondent SOC.
}
