// Copyright 2016 Jacob Chapman

#include <vector>
#include <string>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"
#include "Appliance_Battery.h"

Appliance_Battery::Appliance_Battery() {
}

double Appliance_Battery::rewardFunction(double mostShortage, double binShortage) const {
  return binShortage / mostShortage;
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
  double lengthOfTimestep = SimulationConfig::lengthOfTimestep();
  BatteryDeltaT = lengthOfTimestep / 60.0 / 60.0;
  sumSupply = 0;
  sumShort = 0;
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

void Appliance_Battery::doAction(){

  // qlearning calculate every hour the reward for turning battery on
  // for that hour
  int hourOfTheDay = calculateHourOfDay();
  if (hourOfTheDay != previousHourOfDay) {
    previousHourOfDay = hourOfTheDay;
    // Get the energy needed over the last hour,
    // less what has been supplied by the battery
    double binShortage = sumShort - sumSupply;
    // make sure it is positive and greater than 0
    if (binShortage <= 0 ) {
      binShortage = std::numeric_limits<double>::epsilon();
    }
    // Save the largest ammount of energy needed for comparison
    if (binShortage > mostShortage) {
      mostShortage = binShortage;
    }
    // calculate the percentage of power over the last
    // against the maximum seen
    double reward = rewardFunction(mostShortage, binShortage);
    // make the percentage a fraction

    if(action == 0 && reward > 0.7){
      // if an action did not take place and our reward is above 0.7
      // punish
      reward = -reward;
    } else if (action == 1 && reward < 0.7){
      // if an action did take place and our reward is below 0.7
      // punish
      reward = -reward;
    }
    // save reward for state
    qLearning.setReward(reward);
    qLearning.setState(hourOfTheDay);
    // learn
    qLearning.learn();
    // get new reward
    action = qLearning.getAction();
    // reset the hourly sums
    sumSupply = 0.0;
    sumShort = 0.0;
  }
}

/**
 * @brief The timestep call of the battery appliance
 * @details
 */
void Appliance_Battery::step() {
  doAction();

  // get the recieved power and add it to the battery
  get_new_SOC_charge(received);

  // if the battery is not full calculate how much power is needed
  power = 0;
  if(stateOfCharge < 100) {
    power = get_charge_delta();
  }

  supply = 0.0;
  // If there is a power shortage
  if (powerShortage > 0){
    // Check battery is not empty and there is an action
    // actions are calculated from the learning
    if (action && stateOfCharge > 0) {
      // get power from battery as supply
      supply = get_new_SOC_discharge(powerShortage);
      // we cant supply and request power
      // ie charge and discharge at the same time
      power = 0.0;
      // add the power to the sum for the hour
      // used to learn
      sumSupply += supply;
    }
    // add the power shortage to the hourly sum used for learning
    sumShort += powerShortage;
  }
  // reset the recieved power
  received = 0;
  // save the new state of charge to the datastore
  DataStore::addValue(datastoreIDstateOfCharge, stateOfCharge);
}

void Appliance_Battery::clear() {
  local = false;
  global = false;
  power = 0.0;
  supply = 0.0;
  supplyCost = 0.0;
  receivedCost = 0.0;

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

double Appliance_Battery::get_charge_delta() const {
    double P_request = chargeRate;
    double charge_left = capacity - energy_calc();
    if (charge_left < P_request){
      P_request = charge_left;
    }
    return P_request;
}

double Appliance_Battery::calculateDeltaE(double P_request) const {
  // variation of energy that you will get in the duration of a time step
  return BatteryDeltaT * P_request * efficiency;
}

void Appliance_Battery::calculateStateOfCharge(double energy) {
  stateOfCharge = energy / capacity * 100.0 ; // SOC is fraction of capacities
  if (stateOfCharge > 100){
    stateOfCharge = 100;
  } else if (stateOfCharge < 0){
    stateOfCharge = 0;
  }
}

void Appliance_Battery::get_new_SOC_charge(double P_request) {
    double energy = energy_calc() + calculateDeltaE(P_request); // new capacity iswhat you had plus what you charge
    calculateStateOfCharge(energy);
}

double Appliance_Battery::get_new_SOC_discharge(double P_request) {
    double requestedPower = P_request;
    if (requestedPower > dischargeRate){
      requestedPower = dischargeRate;
    }
    double energy = energy_calc() - calculateDeltaE(requestedPower);; // new capacity, after removing what you discharged
    calculateStateOfCharge(energy);
    return requestedPower;
}

double Appliance_Battery::energy_calc() const{
    return stateOfCharge * capacity / 100; // this is the capacity at the correspondent SOC.
}
