#ifndef Occupant_ACTION_WINDOW_STOCHASTIC_BDI_H
#define Occupant_ACTION_WINDOW_STOCHASTIC_BDI_H

#include "Occupant_Action_Window_Stochastic.hpp"

/**
 * @brief Occupant action on windows using BDI
 * @details Occupant action on windows using BDI adapted from
 * \n Chapman, J., Siebers, P., & Robinson, D. (2017). Data Scarce Behavioural Modelling and the Representation of Social Interactions. Unpublished Manuscript, 1–48.
 */

class Occupant_Action_Window_Stochastic_BDI : public Occupant_Action_Window_Stochastic {
  public:
    Occupant_Action_Window_Stochastic_BDI();
    void setOpenDuringCooking(double OpenDuringCooking);
    void setOpenDuringWashing(double OpenDuringWashing);
    void setOpenDuringSleeping(double OpenDuringSleeping);
    void setDailyMeanTemperature(double dailyMeanTemperature);
    bool doRecipe(const std::vector<double> &activities);
    bool enabled() const;

  private:
    double OpenDuringWashing;
    double OpenDuringCooking;
    double OpenDuringSleeping;
    double dailyMeanTemperature;
};

#endif // Occupant_ACTION_WINDOW_STOCHASTIC_BDI_H
