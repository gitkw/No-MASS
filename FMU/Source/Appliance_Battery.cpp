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
  chargeRate = 500;
  dischargeRate = 600;
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
  get_new_SOC_charge(recieved);
    if(recieved > 0){
        std::cout << "rec " << recieved << std::endl;
    }
  double leninsec = SimulationConfig::lengthOfTimestep();
  double delta_t = leninsec / 60.0 / 60.0;
  power = 0;
  if(stateOfCharge < 100) {
    power = get_charge_delta(delta_t);
  }




  batteryPower += recieved;

  batteryCost += recievedCost;

  recievedCost = 0.0;
  int stepCount = SimulationConfig::getStepCount() % 1440;

  if (stepCount % 24) {
    binShortage = cost;
    if (binShortage > mostShortage) {
      mostShortage = binShortage;
    }
    double reward = 100.0 / mostShortage * binShortage;
    qLearning.setReward(reward);
    qLearning.setState(stepCount % 24);
    qLearning.learn();
    action = qLearning.getAction();
    binShortage = 0;
    cost = 0;
  }

  supplyCost = 0.0;
  supply = 0.0;



  if (action) {
    if (powerShortage > 0 && stateOfCharge > 0) {
      get_new_SOC_discharge(delta_t);
      supply = delta_E;
    }
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

double Appliance_Battery::get_charge_delta(double delta_t) {
    double delta_E = delta_t * P_ch(stateOfCharge); // variation of energy that you will be able to charge in the duration of a time step
    return delta_E;
}

void Appliance_Battery::get_new_SOC_charge(double delta_E) {
    energy = energy_calc(stateOfCharge, capacity) + delta_E; // new capacity iswhat you had plus what you charge
    stateOfCharge = energy / capacity * 100.0 ; // SOC is fraction of capacities
    if (stateOfCharge > 100){
      stateOfCharge = 100;
    }
}

void Appliance_Battery::get_new_SOC_discharge(double delta_t) {
    delta_E = delta_t * P_dis(stateOfCharge); // variation of energy that you will get in the duration of a time step
    energy = energy_calc(stateOfCharge, capacity) - delta_E; // new capacity, after removing what you discharged
    stateOfCharge = energy / capacity * 100.0;
    if (stateOfCharge < 0){
      stateOfCharge = 0;
    }
}

double Appliance_Battery::energy_calc(double SOC, double capacity) {
    return SOC * capacity / 100; // this is the capacity at the correspondent SOC.
}
