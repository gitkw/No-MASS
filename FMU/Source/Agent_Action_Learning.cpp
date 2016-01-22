// Copyright 2015 Jacob Chapman

#include <vector>
// #include "QLearning_PMV.h"
// #include "QLearning_HeatingRate.h"
#include "QLearning_HeatingSetPoints.h"
#include "Agent_Action_Learning.h"

Agent_Action_Learning::Agent_Action_Learning() {
    name = "Learning";
}

void Agent_Action_Learning::setReward(double reward) {
  this->reward = reward;
}

void Agent_Action_Learning::setup(const int id, const int learn) {
  learnId = learn;
  switch (learnId) {
    case 0:

      break;
    case 1:
        ql = std::shared_ptr<QLearning_HeatingSetPoints>(
                  new QLearning_HeatingSetPoints);
      break;
    case 2:
    //  ql = std::shared_ptr<QLearning_PMV>(new QLearning_PMV);
      break;
    case 3:
    //  ql = std::shared_ptr<QLearning_HeatingRate>(new QLearning_HeatingRate);
      break;
  }
  ql->setId(id);
  ql->setup();
}


void Agent_Action_Learning::step(const Zone& zone, bool inZone,
    bool previouslyInZone, const std::vector<double> &activities) {


      switch (learnId) {
        case 0:

          break;
        case 1:

          reward = -0.01;
          if (static_cast<int>(zone.getMeanAirTemperature()) == 20) {
            reward = 0.5;
          }
          if (static_cast<int>(zone.getMeanAirTemperature()) == 22) {
            reward = 0.5;
          }
          if (static_cast<int>(zone.getMeanAirTemperature()) == 21) {
            reward = 0.9;
          }

          break;
        case 2:
        //  ql = std::shared_ptr<QLearning_PMV>(new QLearning_PMV);
          break;
        case 3:
        //  ql = std::shared_ptr<QLearning_HeatingRate>(new QLearning_HeatingRate);
          break;
      }


      ql->setReward(reward);
      result = ql->learn(zone);



      // ql.printQ();
}
