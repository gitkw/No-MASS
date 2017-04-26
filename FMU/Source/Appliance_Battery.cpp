// Copyright 2016 Jacob Chapman

#include <vector>
#include <string>
#include "Configuration.hpp"
#include "DataStore.hpp"
#include "Utility.hpp"
#include "Appliance_Battery.hpp"

Appliance_Battery::Appliance_Battery() {
}

double Appliance_Battery::rewardFunction(double mostShortage, double binShortage) const {
  // make the percentage a fraction
  double reward = binShortage / mostShortage;
  if(action == 0 && reward > 0.7){
    // if an action did not take place and our reward is above 0.7
    // punish
    reward = -reward;
  } else if (action == 1 && reward < 0.7){
    // if an action did take place and our reward is below 0.7
    // punish
    reward = -reward;
  }
  return reward;
}

/**
 * @brief Set up the large appliance model, reading in the large applaince
 * configuration file
 */
void Appliance_Battery::setup(ConfigStructAppliance a) {

  setID(a.id);
  setHoulyPriority(a.priority);
  setBatteryNeighbourhoodDischarge(a.batteryNeighbourhoodDischarge);
  setBatteryNeighbourhoodCharge(a.batteryNeighbourhoodCharge);

  std::string buildingString = "Building" + std::to_string(buildingID);
  std::string s_fullname = buildingString + "_Appliance";
  s_fullname = s_fullname + std::to_string(id) + "_";
  qLearning.setEpsilon(a.epsilon);
  qLearning.setAlpha(a.alpha);
  qLearning.setGamma(a.gamma);
  qLearning.setUpdate(a.update);
  qLearning.setFilename(s_fullname);
  qLearning.setStates(1440 / 60);
  qLearning.setActions(2);
  qLearning.setId(id);
  qLearning.setup();
  double lengthOfTimestep = Configuration::lengthOfTimestep();
  BatteryDeltaT = lengthOfTimestep / 60.0 / 60.0;
  sumSupply = 0;
  sumShort = 0;
  stateOfCharge = 0;
  datastoreIDstateOfCharge = DataStore::addVariable(s_fullname + "stateOfCharge");
  action = false;
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
void Appliance_Battery::stepNeighbourhood() {

  if (batteryNeighbourhoodDischarge){
    if (getSupply() < dischargeRate){
      double dischargeRateOld = dischargeRate;
      dischargeRate -= getSupply();
      calculateSupply();
      dischargeRate = dischargeRateOld;
    }
  }

  if (batteryNeighbourhoodCharge) {
    if (getSupply() == 0 && getReceived() < chargeRate){
      // if the battery is not full calculate how much power is needed
      if(stateOfCharge < 100) {
        setPower(get_charge_delta() - getReceived());
      }
    }
  }
}

/**
 * @brief The timestep call of the battery appliance
 * @details
 */
void Appliance_Battery::step() {
  doAction();

  // get the recieved power and add it to the battery
  get_new_SOC_charge(getReceived());

  setSupply(0.0);
  setSupplyLeft(getSupply());
  calculateSupply();
  setPower(0.0);
  // we cant supply and request power
  // ie charge and discharge at the same time
  if (getSupply() == 0){
    // if the battery is not full calculate how much power is needed
    if(stateOfCharge < 100) {
      setPower(get_charge_delta());
    }
  }
  // reset the recieved power
  setReceived(0.0);
}

void Appliance_Battery::calculateSupply() {
  // If there is a power shortage
  if (powerShortage > 0){
    // Check battery is not empty and there is an action
    // actions are calculated from the learning
    if (action && stateOfCharge > 0) {
      double oldSupply = getSupply();
      double newSupply = get_new_SOC_discharge(powerShortage);
      setSupplyLeft(getSupplyLeft() + newSupply);
      // get power from battery as supply
      setSupply(oldSupply + newSupply);

      // add the power to the sum for the hour
      // used to learn
      sumSupply += newSupply;
    }
    // add the power shortage to the hourly sum used for learning
    sumShort += powerShortage;
  }
}

void Appliance_Battery::clear() {
  // save the new state of charge to the datastore
  DataStore::addValue(datastoreIDstateOfCharge, stateOfCharge);
  local = false;
  global = false;
  setPower(0.0);
  setSupply(0.0);
  setSupplyCost(0.0);
  setReceivedCost(0.0);
  setSupplyLeft(0.0);
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

double Appliance_Battery::powerAvaliable(const double energy) const {
  // power in battery for energy provided
  return energy / BatteryDeltaT / efficiency;
}

double Appliance_Battery::get_new_SOC_discharge(double P_request) {
    double requestedPower = P_request;
    if (requestedPower > dischargeRate){
      requestedPower = dischargeRate;
    }
    double currentEnergy = energy_calc();
     // new capacity, after removing what you discharged;
    double energy = currentEnergy - calculateDeltaE(requestedPower);
    if (energy < 0) {
      energy = 0;
      requestedPower = powerAvaliable(currentEnergy);
    }
    calculateStateOfCharge(energy);
    return requestedPower;
}

/**
 * calculates the current energy available
 * @return current energy
 */
double Appliance_Battery::energy_calc() const{
    return stateOfCharge * capacity / 100; // this is the capacity at the correspondent SOC.
}

void Appliance_Battery::saveNeighbourhoodCalculate() {
  if (batteryNeighbourhoodDischarge){
    parametersNeighbourhood.supply = parameters.supply - parameters.suppliedLeft;
    if (parametersLocal.supply > 0 ){
      parametersNeighbourhood.supply = parameters.supply - parametersLocal.supply - parametersLocal.suppliedLeft;
    }
    parametersNeighbourhood.suppliedLeft = parameters.suppliedLeft;
  }
  if (batteryNeighbourhoodCharge) {
    parametersNeighbourhood.received = 0;
    parametersNeighbourhood.receivedCost = 0;
    parametersNeighbourhood.power = 0;
    if (parameters.received > 0) {
      parametersNeighbourhood.received = parameters.received - parametersLocal.received;
        parametersNeighbourhood.receivedCost = parameters.receivedCost - parametersLocal.receivedCost;
      parametersNeighbourhood.power = parameters.power;
    }
  }
}

void Appliance_Battery::saveGlobalCalculate(){}

void Appliance_Battery::setBatteryNeighbourhoodDischarge(bool batteryNeighbourhoodDischarge) {
  this->batteryNeighbourhoodDischarge = batteryNeighbourhoodDischarge;
}

void Appliance_Battery::setBatteryNeighbourhoodCharge(bool batteryNeighbourhoodCharge) {
  this->batteryNeighbourhoodCharge = batteryNeighbourhoodCharge;
}

double Appliance_Battery::getStateOfCharge() const {
  return stateOfCharge;
}
