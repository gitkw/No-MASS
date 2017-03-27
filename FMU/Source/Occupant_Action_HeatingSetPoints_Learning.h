#ifndef Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
#define Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
#include <memory>
#include "QLearning_Occupant.h"
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

    void parseConfiguration(const std::string filename);

    QLearning_Occupant qlWeekDay;
    QLearning_Occupant qlWeekEnd;

    int learnId;
    int agentId;

    int previousHour;
    double setPoint;
    double steps;
    bool hasBeenInZone;
    double pmv;
    int pmv_name;
    int step_name;

    std::string file;
    std::vector<std::vector<int>> stateMappings;

    int getState() const;
};

#endif // Occupant_ACTION_HEATINGSETPOINTS_LEARNING_H
