// Copyright 2016 Jacob Chapman
#ifndef OCCUPANT_ACTION_HEATINGSETPOINTS_LEARNING_H
#define OCCUPANT_ACTION_HEATINGSETPOINTS_LEARNING_H

#include <memory>
#include "QLearning.hpp"
#include "Occupant_Action.hpp"

/**
 * @brief
 * @details
 */

class Occupant_Action_HeatingSetPoints_Learning : public Occupant_Action {
  public:
    Occupant_Action_HeatingSetPoints_Learning();
    void step(const Building_Zone& zone, const bool inZone);
    void setup(const int id, const int learn);
    void print();
    void reset();
    void setFile(std::string file);
  private:

    void parseConfiguration(const std::string &filename);
    int getState() const;

    QLearning qlWeekDay;
    QLearning qlWeekEnd;

    int learnId;
    int agentId;
    int previousHour;
    double setPoint;
    double steps;

    double pmv;
    bool hasBeenInZone;
    int pmv_name;
    int step_name;

    std::string file;
    std::vector<std::vector<int>> stateMappings;

};

#endif // OCCUPANT_ACTION_HEATINGSETPOINTS_LEARNING_H
