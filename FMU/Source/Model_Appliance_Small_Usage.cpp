// Copyright 2016 Jacob Chapman

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "Utility.h"
#include "SimulationConfig.h"
#include "Model_Appliance_Small_Usage.h"

Model_Appliance_Small_Usage::Model_Appliance_Small_Usage() {}

double Model_Appliance_Small_Usage::consumption(const int timeStep) {
  double result = 0.0;
  int leninsec = SimulationConfig::lengthOfTimestep();
  duration = duration - leninsec * 60;
  if (duration <= 0) {
    int ten = static_cast<int>(((timeStep * leninsec) / 360.0)) % 144;
    state = calculateStateAtTenMin(ten);
    duration = durationAtState(state);
  }
  result += getFractionalPowerAtState(state);
  return result;
}


double Model_Appliance_Small_Usage::getFractionalPowerAtState(int state) const {
  return fractions[state] *
          (ratedPower * (SimulationConfig::lengthOfTimestep() / 60) / 60);
}

void Model_Appliance_Small_Usage::setFolderLocation(
                                            const std::string &folderLocation) {
    this->folderLocation = folderLocation;
}

double Model_Appliance_Small_Usage::durationAtState(int state) const {
  return weibullInvCdf(weibullLoc[state], weibullShape[state],
                        weibullScale[state]);
}

int Model_Appliance_Small_Usage::calculateStateAtTenMin(int timeAsInt) const {
  return Utility::cumulativeProbability(stateProbabilities[timeAsInt]);
}

void Model_Appliance_Small_Usage::readFractions(const std::string &file) {
  std::string fileName = folderLocation + file;
  Utility::uTable<double> para = Utility::csvToTable<double>(fileName);
  for (auto p : para) {
    fractions.push_back(p[1]);
  }
}

void Model_Appliance_Small_Usage::readSumRatedPowers(const std::string &file) {
  std::string fileName = folderLocation + file;
  Utility::uTable<double> para = Utility::csvToTable<double>(fileName);
  for (auto p : para) {
    sumRatedPowers.push_back(p[0]);
  }
}

void Model_Appliance_Small_Usage::readStateProbabilities(
                                                    const std::string &file) {
  std::string fileName = folderLocation + file;
  stateProbabilities = Utility::csvToTable<double>(fileName, true);
}

void Model_Appliance_Small_Usage::readWeibullParameters(
                                                    const std::string &file) {
  std::string fileName = folderLocation + file;
  Utility::uTable<double> para = Utility::csvToTable<double>(fileName, true);
  weibullLoc = para[0];
  weibullShape = para[1];
  weibullScale = para[2];
}

void Model_Appliance_Small_Usage::setRatedPowerAt(const int i) {
  ratedPower = sumRatedPowers[i];
}

double Model_Appliance_Small_Usage::weibullInvCdf(float loc,
                                            float shape, float scale) const {
    /*"""
    Inverse value of the CDF. For a random number between 0 and 1, gives the duration of use of appliance, comparing it
    against CDF.
    :param loc: loc parameter - it does not influence
    :param shape:
    :param scale:
    :return:
    """
    */
    double random = randomDouble();
    return  loc + pow(-pow(scale, shape) * log(1-random), (1/shape));
}
