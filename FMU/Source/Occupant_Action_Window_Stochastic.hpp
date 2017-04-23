#ifndef Occupant_ACTION_WINDOW_STOCHASTIC_H
#define Occupant_ACTION_WINDOW_STOCHASTIC_H

#include "Occupant_Action_Window.hpp"
#include "Model_Windows.hpp"
#include "Building_Zone.hpp"

/**
 * @brief Occupant action on windows using stochastic model
 * @details Occupant action on windows using stochastic model
 */

class Occupant_Action_Window_Stochastic : public Occupant_Action_Window {
    public:
        Occupant_Action_Window_Stochastic();
        void setup(int windowID, int id);
        void step(const Building_Zone& zone, const bool inZone,
                  const bool previouslyInZone,
                  const std::vector<double> &activities);
};

#endif // Occupant_ACTION_WINDOW_STOCHASTIC_H
