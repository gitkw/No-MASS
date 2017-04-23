// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_OCCUPANT_ACTION_LIGHTS_BDI_H_
#define FMU_SOURCE_OCCUPANT_ACTION_LIGHTS_BDI_H_

#include <vector>
#include "Occupant_Action_Lights.hpp"

/**
 * @brief Occupant action on lights using BDI
 * @details Occupant action on lights using BDI adapted from
 * \n Chapman, J., Siebers, P., & Robinson, D. (2017). Data Scarce Behavioural Modelling and the Representation of Social Interactions. Unpublished Manuscript, 1–48.
 */

class Occupant_Action_Lights_BDI : public Occupant_Action_Lights {
 public:
    Occupant_Action_Lights_BDI();

    void setOffDuringSleep(double OffDuringSleep);
    void setOffDuringAudioVisual(double OffDuringAudioVisual);
    bool doRecipe(const std::vector<double> &activities);

 private:
    double OffDuringSleep;
    double OffDuringAudioVisual;
};

#endif  // FMU_SOURCE_OCCUPANT_ACTION_LIGHTS_BDI_H_
