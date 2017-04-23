// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_OCCUPANT_ACTION_SHADES_H_
#define FMU_SOURCE_OCCUPANT_ACTION_SHADES_H_

#include <vector>
#include "Occupant_Action.hpp"
#include "Model_ExternalShading.hpp"

/**
 * @brief Occupant action on shades
 * @details Occupant action on shades
 */

class Occupant_Action_Shades : public Occupant_Action {
 public:
  Occupant_Action_Shades();
  void setup(int windowID);
  void setIndoorIlluminance(const float lumint);
  void step(const Building_Zone& zone, const bool inZone,
            const bool previouslyInZone);

 protected:
   float Lumint = 0;

 private:
  Model_ExternalShading m_blindUsage;

};

#endif  // FMU_SOURCE_OCCUPANT_ACTION_SHADES_H_
