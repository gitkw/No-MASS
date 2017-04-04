// Copyright 2015 Jacob Chapman

#include <cstring>
#include <string>
#include <vector>
#include "Utility.h"
#include "SimulationConfig.h"
#include "Model_Appliance_Large_Usage_Survival.h"


Model_Appliance_Large_Usage_Survival::Model_Appliance_Large_Usage_Survival() {
  state = Utility::randomInt(0.0, 1.0);
  duration = 0;
}
void Model_Appliance_Large_Usage_Survival::decreaseDuration() {
  duration -= SimulationConfig::lengthOfTimestep() / 60.0;
}

bool Model_Appliance_Large_Usage_Survival::onAt(const int timeStep) {
  on = false;
  if (duration <= 0) {
    double probability = probOn(timeStep);
    double random = Utility::randomDouble(0.0, 1.0);
    if (random < probability) {
      on = true;
      duration = randomWeibull();
    }
  } else {
    on = true;
  }
  return on;
}

double Model_Appliance_Large_Usage_Survival::randomWeibull() const {
    // Draws a random number from a Weibull distribution,
    // proceeding by inversion of the Weibull cdf, defined as
    // F(x) = 1 - exp(-(x/scale)^shape)
    // double x = randomUniform(0.f,1.f);
    // double y = scale*pow(-log(1.f-random),1.f/shape);
    double random = randomDouble();
    return pow(-log(random), 1 / shape) / scale;
    //  return scale * pow(-log(1.f-random), 1.f / shape);
}

void Model_Appliance_Large_Usage_Survival::setShape(double shape) {
  this->shape = shape;
}
void Model_Appliance_Large_Usage_Survival::setScale(double scale) {
  this->scale = scale;
}

void Model_Appliance_Large_Usage_Survival::parseShapeScale(
                                          rapidxml::xml_node<> *node) {
  if (std::strcmp(node->name(), "shape") == 0) {
      shape = 1.0 / std::stod(node->value());
  } else if (std::strcmp(node->name(), "scale") == 0) {
      scale = exp(std::stod(node->value()));
  }
}

void Model_Appliance_Large_Usage_Survival::setDuration(double duration){
  this->duration = duration;
}
