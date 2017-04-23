// Copyright 2015 Jacob Chapman

#include <vector>
#include <string>
#include <utility>
#include "Utility.hpp"
#include "DataStore.hpp"
#include "Model_Activity_Survival.hpp"

/**
 * sleep
 * passive
 * Audio / visul
 * IT
 * Cooking
 * Cleaning
 * Other Washing
 * Metabolic
 * Washing applicance
 * out
 */


Model_Activity_Survival::Model_Activity_Survival() {
  duration = 0;
}

int Model_Activity_Survival::multinominalActivity(const double p[24][10],
                                                    const int hourCount) {
  duration -= Configuration::lengthOfTimestep() / 60.0;
  if (duration < 1.0) {
    state = Utility::cumulativeProbability(p[hourCount], 10);
    double shape = shapes.at(hourCount +1)[state];
    double scale = scales.at(hourCount +1)[state];
    duration = this->randomWeibull(exp(scale), 1/shape);
  }
  return state;
}

void Model_Activity_Survival::parseOther(rapidxml::xml_node<> *node) {
  std::string type = node->name();
  bool isScale = type.find("scale") != std::string::npos;
  type.erase(0, 5);
  int hour = std::stoi(type);
  std::vector<double> items;
  std::vector<std::string> tokProbs;
  Utility::splitCSV(node->value(), &tokProbs);
  for (std::string strProb : tokProbs) {
      items.push_back(std::stod(strProb));
  }
  std::pair<int, std::vector<double>> y(hour, items);
  if (isScale) {
    scales.insert(y);
  } else {
    shapes.insert(y);
  }
}

double Model_Activity_Survival::randomWeibull(double scale,
                                                      double shape) const {
    // Draws a random number from a Weibull distribution,
    // proceeding by inversion of the Weibull cdf, defined as
    // F(x) = 1 - exp(-(x/scale)^shape)
    // double x = randomUniform(0.f,1.f);
    // double y = scale*pow(-log(1.f-random),1.f/shape);
    double random = randomDouble();
    return pow(-log(random), 1 / shape) / scale;
    //  return scale * pow(-log(1.f-random), 1.f / shape);
}
