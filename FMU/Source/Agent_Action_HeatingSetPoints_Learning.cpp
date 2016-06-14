// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
// #include "QLearning_PMV.h"
// #include "QLearning_HeatingRate.h"
#include "DataStore.h"
#include "QLearning_HeatingSetPoints.h"
#include "Agent_Action_HeatingSetPoints_Learning.h"

Agent_Action_HeatingSetPoints_Learning::Agent_Action_HeatingSetPoints_Learning() {
    setPoint = 20;
    pmv = -1.0;
    result = 20;
}

void Agent_Action_HeatingSetPoints_Learning::print() {
  qlWeekDay.printQ();
  qlWeekEnd.printQ();
}
void Agent_Action_HeatingSetPoints_Learning::reset() {
  qlWeekDay.reset();
  qlWeekEnd.reset();
}

void Agent_Action_HeatingSetPoints_Learning::setup(const int id, const int learn) {
  learnId = learn;
  agentId = id;
  switch (learnId) {
    case 0:

      break;
    case 1:

        std::string zoneIdStr = std::to_string(zoneId);
        qlWeekDay.setFilename("Weekday-" + zoneIdStr + "-");
        qlWeekEnd.setFilename("Weekend-" + zoneIdStr + "-");
        qlWeekDay.setStates(24 * 12);
        qlWeekEnd.setStates(24 * 12);
        qlWeekDay.setId(id);
        qlWeekDay.setup();
        qlWeekEnd.setId(id);
        qlWeekEnd.setup();
        pmv_name = "Weekday-" + zoneIdStr + "-_pmv" + std::to_string(id);
        DataStore::addVariable(pmv_name);
        step_name = "Weekday-" + zoneIdStr + "-_steps" + std::to_string(id);
        DataStore::addVariable(step_name);
      break;
    /*case 2:
    //  ql = std::shared_ptr<QLearning_PMV>(new QLearning_PMV);
      break;
    case 3:
    //  ql = std::shared_ptr<QLearning_HeatingRate>(new QLearning_HeatingRate);
      break;*/
  }
}

void Agent_Action_HeatingSetPoints_Learning::step(const Building_Zone& zone, const bool inZone) {
    int hour = DataStore::getValue("hour");

    if (inZone) {
      hasBeenInZone = true;
      setPoint = zone.getHeatingState();
      pmv = reward;
    }

    if (hour != previousHour) {
      reward = 0;
      steps = steps + 1;
      if (hasBeenInZone) {
        steps = 0;
      }

      bool x = (pmv > 0 && setPoint == 10);
      bool w = (pmv > 0 && setPoint > 10);
      bool y = (pmv < -0.5);
      bool z = (pmv >= -0.5 && pmv <= 0);
      bool a = setPoint == 10 && steps > 1;

      reward = hasBeenInZone * (1 * z + 0.1 * x - 0.1 * y - 0.1 * w) +
                (1-hasBeenInZone) * (a * 0.1 + (1-a) * -0.1);

      int day = DataStore::getValue("day")+1;
      int dayOfTheWeek = (day - 1) % 7;

      if (dayOfTheWeek < 5) {
        qlWeekDay.setAction(setPoint-10);
        qlWeekDay.setReward(reward);
        result = qlWeekDay.learn() + 10;
      } else {
        qlWeekEnd.setAction(setPoint-10);
        qlWeekEnd.setReward(reward);
        result = qlWeekEnd.learn() + 10;
      }

      DataStore::addValue(pmv_name, pmv);
      DataStore::addValue(step_name, steps);
      hasBeenInZone = false;
      previousHour = hour;
      setPoint = result;
    }
}
