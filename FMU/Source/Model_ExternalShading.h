/*
 * File:   Model_BlindUsage.h
 * Author: jake
 *
 * Created on September 28, 2013, 1:27 PM
 */

#ifndef MODEL_BLINDUSAGE_H
#define	MODEL_BLINDUSAGE_H

#include "Model_RandomWeibull.h"

class Model_ExternalShading : public Model_RandomWeibull {
public:
    Model_ExternalShading();
    //void calculate(double state, double Lumint, double Evg, bool currentlyOccupied, bool previouslyOccupied);
    double arrival(double state, double Lumint, double Evg);
    double departure(double state, double Lumint, double Evg);
    double intermediate(bool state, double Lumint, double Evg);

private:
    double arrivalRaising(double state, double Lumint, double Evg);
    double arrivalLowering(double state,double Lumint, double Evg);
    double departureLowering(double state,double Lumint, double Evg);
    double departureRaising(double state,double Lumint, double Evg);

};

#endif	/* MODEL_BLINDUSAGE_H */
