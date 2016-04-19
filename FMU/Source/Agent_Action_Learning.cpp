// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include <cmath>
// #include "QLearning_PMV.h"
// #include "QLearning_HeatingRate.h"
#include "DataStore.h"
#include "QLearning_HeatingSetPoints.h"
#include "Agent_Action_Learning.h"

Agent_Action_Learning::Agent_Action_Learning() {
    name = "Learning";
    setPoint = 20;
    pmv = -1;
    result = 20;
}

void Agent_Action_Learning::setReward(const double reward) {
  this->reward = reward;
}

void Agent_Action_Learning::setZoneId(const double zoneId) {
  this->zoneId = zoneId;
}

void Agent_Action_Learning::print() {
  qlWeekDay->printQ();
  qlWeekEnd->printQ();
}
void Agent_Action_Learning::reset() {
  qlWeekDay->reset();
  qlWeekEnd->reset();
}

void Agent_Action_Learning::setup(const int id, const int learn) {
  learnId = learn;
  agentId = id;
  switch (learnId) {
    case 0:

      break;
    case 1:
        qlWeekDay = std::shared_ptr<QLearning_HeatingSetPoints>(
                  new QLearning_HeatingSetPoints());
        qlWeekEnd = std::shared_ptr<QLearning_HeatingSetPoints>(
                  new QLearning_HeatingSetPoints());
        qlWeekDay->setFilename("Weekday-" + std::to_string(zoneId) + "-");
        qlWeekEnd->setFilename("Weekend-" + std::to_string(zoneId) + "-");
        qlWeekDay->setStates(24 * 12);
        qlWeekEnd->setStates(24 * 12);
        qlWeekDay->setId(id);
        qlWeekDay->setup();
        qlWeekEnd->setId(id);
        qlWeekEnd->setup();
      break;
    case 2:
    //  ql = std::shared_ptr<QLearning_PMV>(new QLearning_PMV);
      break;
    case 3:
    //  ql = std::shared_ptr<QLearning_HeatingRate>(new QLearning_HeatingRate);
      break;
  }
}

void Agent_Action_Learning::step(const Building_Zone& zone, bool inZone,
    bool previouslyInZone, const std::vector<double> &activities) {

    int hour = DataStore::getValue("hour");

    if(inZone){
      hasBeenInZone = true;
      setPoint = zone.getHeatingState();
      pmv = reward;
    }

    if(hour != previousHour){
      reward = 0;
      if(hasBeenInZone){
        if(pmv > -0.5 && pmv <= 0){
          reward = 1;
        }else{
          reward = -0.1;
        }
        steps = 0;
      } else {
        if(setPoint == 10 && steps > 1){
          reward = 1;
        }else{
          reward = -0.1;
        }
        steps = steps + 1;
      }

      int day = DataStore::getValue("day")+1;
      int dayOfTheWeek = (day - 1) % 7;
      if(dayOfTheWeek < 5){
        qlWeekDay->setHeatingSetPoint(setPoint-10);
        qlWeekDay->setReward(reward);
        result = qlWeekDay->learn(zone) + 10;
      }else{
        qlWeekEnd->setHeatingSetPoint(setPoint-10);
        qlWeekEnd->setReward(reward);
        result = qlWeekEnd->learn(zone) + 10;
      }
      hasBeenInZone = false;
      previousHour = hour;
      setPoint = result;
    }
}
