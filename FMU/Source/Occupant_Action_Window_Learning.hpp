#ifndef OCCUPANT_ACTION_WINDOW_LEARNING_H
#define OCCUPANT_ACTION_WINDOW_LEARNING_H

#include <memory>
#include "QLearning.hpp"
#include "Occupant_Action_Window.hpp"
#include "Model_Windows.hpp"
#include "Building_Zone.hpp"

/**
 * @brief Occupant action on windows using Q-Learing model
 * @details Occupant action on windows using Q-Learing model
 */

class Occupant_Action_Window_Learning : public Occupant_Action_Window {
  public:
    void setup(const int id);
    void step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone);
    void print();
    void reset();

  private:
    QLearning learn;
    int window_name;
};

#endif // OCCUPANT_ACTION_WINDOW_LEARNING_H
