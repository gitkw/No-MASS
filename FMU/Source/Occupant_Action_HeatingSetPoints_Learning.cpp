// Copyright 2015 Jacob Chapman

#include <string>
#include "DataStore.h"
#include "SimulationTime.h"
#include "QLearning_HeatingSetPoints.h"
#include "Occupant_Action_HeatingSetPoints_Learning.h"

Occupant_Action_HeatingSetPoints_Learning::
                  Occupant_Action_HeatingSetPoints_Learning() {
    setPoint = 20;
    pmv = -1.0;
    result = 20;
}

void Occupant_Action_HeatingSetPoints_Learning::print() {
  qlWeekDay.printQ();
  qlWeekEnd.printQ();
}
void Occupant_Action_HeatingSetPoints_Learning::reset() {
  qlWeekDay.reset();
  qlWeekEnd.reset();
}

void Occupant_Action_HeatingSetPoints_Learning::setup(const int id,
                                                      const int learn) {
  learnId = learn;
  agentId = id;
  std::string zoneIdStr = std::to_string(zoneId);
  qlWeekDay.setFilename("Weekday-" + zoneIdStr + "-");
  qlWeekEnd.setFilename("Weekend-" + zoneIdStr + "-");
  qlWeekDay.setStates(24 * 12);
  qlWeekEnd.setStates(24 * 12);
  qlWeekDay.setId(id);
  qlWeekDay.setup();
  qlWeekEnd.setId(id);
  qlWeekEnd.setup();
  pmv_name =
  DataStore::addVariable("Weekday-" + zoneIdStr + "-_pmv" + std::to_string(id));
  step_name =
  DataStore::addVariable("Weekday-" + zoneIdStr
    + "-_steps" + std::to_string(id));
}

void Occupant_Action_HeatingSetPoints_Learning::step(const Building_Zone& zone,
                                                      const bool inZone) {
    int hour = SimulationTime::hour;

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

      int day = SimulationTime::day + 1;
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
