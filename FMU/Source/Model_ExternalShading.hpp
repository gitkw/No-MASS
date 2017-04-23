// Copyright 2016 Jacob Chapman

#ifndef MODEL_BLINDUSAGE_H
#define	MODEL_BLINDUSAGE_H

#include "Model_RandomWeibull.hpp"

/**
 * @brief Models the prediction of external shades
 * @details Models the prediction of external shades adapted from
 * \n Haldi, F., & Robinson, D. (2010). Adaptive actions on shading devices in response to local visual stimuli. Journal of Building Performance Simulation, 3(2), 135–153. https://doi.org/10.1080/19401490903580759
 */

class Model_ExternalShading : public Model_RandomWeibull {
public:
    Model_ExternalShading();
    //void calculate(double state, double Lumint, double Evg, bool currentlyOccupied, bool previouslyOccupied);
    double arrival(double state, double Lumint, double Evg);
    double departure(double state, double Lumint, double Evg);
    double intermediate(bool state, double Lumint, double Evg);

    void setFullVars(float afullraise, float boutfullraise, float bsfullraise, float bsfulllower, float boutfulllower, float afulllower);
    void setDurationVars(float aSFlower, float bSFlower, float shapelower);
    void setArrivalVars(float a01arr, float b01inarr, float b01sarr, float a10arr, float b10inarr, float b10sarr);
    void setInterVars(float a01int, float b01inint, float b01sint, float a10int, float b10inint, float b10sint);


private:
    double arrivalRaising(double state, double Evg);
    double arrivalLowering(double state, double Evg);
    double departureLowering(double state, double Evg);
    double departureRaising(double state, double Evg);
    // Probability of lowering on arrival
    float a01arr;
    float b01inarr;
    float b01sarr;
    // Probability of raising on arrival
    float a10arr;
    float b10inarr;
    float b10sarr;
    // Probability of lowering during presence
    float a01int;
    float b01inint;
    float b01sint;
    // Probability of raising during presence
    float a10int;
    float b10inint;
    float b10sint;
    // Probability of full raising
    float afullraise;
    float boutfullraise;
    float bsfullraise;
    // Choice of new unshaded fraction
    float aSFlower;
    float bSFlower;
    float shapelower;

    float bsfulllower;
    float boutfulllower;
    float afulllower;


};

#endif	/* MODEL_BLINDUSAGE_H */
