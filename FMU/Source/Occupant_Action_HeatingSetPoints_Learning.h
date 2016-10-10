#ifndef Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
#define Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
#include <memory>
#include "QLearning_HeatingSetPoints.h"
#include "Occupant_Action.h"

class Occupant_Action_HeatingSetPoints_Learning : public Occupant_Action
{
  public:
    Occupant_Action_HeatingSetPoints_Learning();
    void step(const Building_Zone& zone, const bool inZone);
    void setup(const int id, const int learn);
    void print();
    void reset();
  private:
    QLearning_HeatingSetPoints qlWeekDay;
    QLearning_HeatingSetPoints qlWeekEnd;

    int learnId;
    int agentId;

    int previousHour;
    double setPoint;
    double steps;
    bool hasBeenInZone;
    double pmv;
    int pmv_name;
    int step_name;
  //  QLearning ql;
};

#endif // Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
