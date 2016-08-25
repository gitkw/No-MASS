// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_LEARNING_H_
#define APPLIANCE_LARGE_LEARNING_H_

#include <vector>
#include <string>
#include "QLearning_Appliance.h"
#include "Appliance_Large.h"

/**
 * @brief Large appliances learning class
 * @details This will handle the appliance learning model for profile shifting
 */

class Appliance_Large_Learning : public Appliance_Large {
 public:
  Appliance_Large_Learning();
  void setup();
  void step();
  void postprocess();
  void addToCost(const double cost);
  void setEpsilon(double epsilon);
  void setAlpha(double alpha);
  void setGamma(double gamma);
  void setUpdate(bool update);

 private:
  bool isLearningPeriod;
  bool isWaitingForApplianceToStart;
  int startTime;
  unsigned int learningStep;
  unsigned int nonLearningStep;
  double cost;
  std::string s_fullname_actual;
  std::vector<double> powerProfile;
  QLearning_Appliance qLearning;

  void stepApplianceOffAndNotLearning();
  void calculateProfile();
  void calculateLearntStartTime();
  void startLearningPeriod(const int hourOfTheDay);
  void stopLearningPeriod(const int hourOfTheDay);
  void saveActualProfile();
  int calculateHourOfDay() const;
  double calculateReward();

  double epsilon;   // probability of a random action selection
  double alpha;     // learning rate
  double gamma;     // discount factor (how soon do you care)
  bool update;
};

#endif  // APPLIANCE_LARGE_LEARNING_H_
