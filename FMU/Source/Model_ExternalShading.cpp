/*
 * File:   Model_BlindUsage.cpp
 * Author: jake
 *
 * Created on September 28, 2013, 1:27 PM
 */
#include <cmath>
#include <algorithm>
#include "Utility.h"
#include "Model_ExternalShading.h"

Model_ExternalShading::Model_ExternalShading() {
}
/*
void Model_ExternalShading::calculate(double state, double Lumint, double Evg, bool currentlyOccupied, bool previouslyOccupied) {
    // This function simulates the occurence of actions on lower shading devices (covering the
    // vision window). If an action is simulated, the chosen unshaded fraction is then predicted.
    // Input parameters are: indoor workplane illuminance (Lumint) and outdoor global horizontal
    // illuminance (Evg).
    // 1 is open
    // 0 is closed
    previousShadingState = state;
    //std::cout << "previous blind state: " << state << std::endl;

    // --- Simulation of lower blinds -------------------------------------------------------------

    if (!currentlyOccupied && !previouslyOccupied) {
        currentShadingState = previousShadingState;
    } else if (currentlyOccupied && !previouslyOccupied) {
        arrival(Lumint, Evg);
    } else {
        departure(Lumint, Evg);
    }
    return;
}
*/

double Model_ExternalShading::arrival(double state, double Lumint, double Evg) {
    double currentShadingState;
    // Probability of lowering on arrival
    float a01arr_LB = -7.41;
    float b01inarr_LB = 0.001035;
    float b01sarr_LB = 2.17;
    // Probability of raising on arrival
    float a10arr_LB = -1.520;
    float b10inarr_LB = -0.000654;
    float b10sarr_LB = -3.139;
    double probraise = 0.f;
    if (state != 1.f) {
        double m_raise = a10arr_LB + b10inarr_LB * Lumint + b10sarr_LB * (state);
        probraise = probability(m_raise);
    }
    double problower = 0.f;
    if (state != 0.f) {
        double m_lower = a01arr_LB + b01inarr_LB * Lumint + b01sarr_LB * (state);
        problower = probability(m_lower);
    }
    if (problower >= probraise) {
        if (randomDouble() < problower) {
            currentShadingState = arrivalLowering(state, Lumint, Evg);
        } else if (randomDouble() < probraise) {
            currentShadingState = arrivalRaising(state, Lumint, Evg);
        } else {
            currentShadingState = state;
        }
    } else {
        if (randomDouble() < probraise) {
            currentShadingState = arrivalRaising(state, Lumint, Evg);
        } else if (randomDouble() < problower) {
            currentShadingState = arrivalLowering(state, Lumint, Evg);
        } else {
            currentShadingState = state;
        }
    }
    return currentShadingState;
}


double Model_ExternalShading::intermediate(bool state, double Lumint, double Evg){

    return departure(state, Lumint, Evg);
}

double Model_ExternalShading::departure(double state, double Lumint, double Evg) {
    double currentShadingState;
    // Probability of lowering during presence
    float a01int_LB = -8.013;
    float b01inint_LB = 0.000841;
    float b01sint_LB = 1.270;
    // Probability of raising during presence
    float a10int_LB = -3.625;
    float b10inint_LB = -0.000276;
    float b10sint_LB = -2.683;

    float problower = 0.f;
    float probraise = 0.f;

    if (state != 1.f) {
        double m_probraise = a10int_LB + b10inint_LB * Lumint + b10sint_LB * (state);
        probraise = probability(m_probraise);
    }
    if (state != 0.f) {
        double m_problower = a01int_LB + b01inint_LB * Lumint + b01sint_LB * (state);
        problower = probability(m_problower);
    }

    if (problower >= probraise) {
        if (randomDouble() < problower) {
            currentShadingState = departureLowering(state, Lumint, Evg);
        } else if (randomDouble() < probraise) {
            currentShadingState = departureRaising(state, Lumint, Evg);
        } else {
            currentShadingState = state;
        }
    } else {
        if (randomDouble() < probraise) {
            currentShadingState = departureRaising(state, Lumint, Evg);
        } else if (randomDouble() < problower) {
            currentShadingState = departureLowering(state, Lumint, Evg);
        } else {
            currentShadingState = state;
        }
    }
    return currentShadingState;
}

double Model_ExternalShading::arrivalRaising(double state, double Lumint, double Evg) {
    double currentShadingState;
    // Probability of full raising
    float afullraise_LB = 0.435;
    float boutfullraise_LB = 1.95;
    float bsfullraise_LB = -0.0000231;

    double m_totraise = afullraise_LB + boutfullraise_LB * Evg + bsfullraise_LB * (state);
    float ptotraise = probability(m_totraise);
    double r = randomDouble();
    if (r < ptotraise) {
        currentShadingState = 1.f;
    } else {
        currentShadingState = 0.01f * round(100.f * randomDouble((state), 1.f));
    }
    return currentShadingState;
}

double Model_ExternalShading::arrivalLowering(double state, double Lumint, double Evg) {
    double currentShadingState;
    // Probability of full lowering
    float afulllower_LB = -0.27;
    float boutfulllower_LB = -2.23;
    float bsfulllower_LB = 0.00000091;
    double m_totlow = afulllower_LB + boutfulllower_LB * Evg + bsfulllower_LB * (state);
    float ptotlow = probability(m_totlow);
    if (randomDouble() < ptotlow) {
        currentShadingState = 0.f;
    } else {
        // Choice of new unshaded fraction
        float aSFlower = -2.294;
        float bSFlower = 1.522;
        float shapelower = 1.708;
        float Reduction = randomWeibull(exp(aSFlower + bSFlower * (state)), shapelower);
        currentShadingState = (0.01f * round(100.f * std::max((state) - Reduction, 0.01)));
    }
    return currentShadingState;
}

double Model_ExternalShading::departureLowering(double state, double Lumint, double Evg) {
    double currentShadingState;
    float bsfulllower_LB = 0.00000091;
    float boutfulllower_LB = -2.23;
    float afulllower_LB = -0.27;
    double m_ptotlow = afulllower_LB + boutfulllower_LB * Evg + bsfulllower_LB * (state);
    float ptotlow = probability(m_ptotlow);
    if (randomDouble() < ptotlow) {
        currentShadingState = (0.f);
    } else {
        float aSFlower = -2.294;
        float bSFlower = 1.522;
        float shapelower = 1.708;
        float Reduction = randomWeibull(exp(aSFlower + bSFlower * (state)), shapelower);
        currentShadingState = (0.01f * round(100.f * std::max((state) - Reduction, 0.01)));
    }
    return currentShadingState;
}

double Model_ExternalShading::departureRaising(double state, double Lumint, double Evg) {
    double currentShadingState;
    // Probability of full raising
    float afullraise_LB = 0.435;
    float boutfullraise_LB = 1.95;
    float bsfullraise_LB = -0.0000231;

    double m_totraise = afullraise_LB + boutfullraise_LB * Evg + bsfullraise_LB * (state);
    float ptotraise = probability(m_totraise);

    if (randomDouble(0.f, 1.f) < ptotraise) {
        currentShadingState = (1.f);
    } else {
        currentShadingState = (0.01f * round(100.f * randomDouble((state), 1.f)));
    }
    return currentShadingState;
}
