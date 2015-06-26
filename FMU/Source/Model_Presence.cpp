/*
 * File:   Model_Presence.cpp
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#include <ctime>
#include <vector>
#include <cassert>
#include <algorithm>
#include "Utility.h"
#include "SimulationConfig.h"
#include "Model_Presence.h"

Model_Presence::Model_Presence() {
}

Model_Presence::Model_Presence(const Model_Presence &orig) {
}

Model_Presence::~Model_Presence() {
}

std::vector<int> Model_Presence::calculatePresenceFromActivities(const std::vector<double> activities) {
    std::vector<int> p;
    for(double activity: activities) {
        if (activity != 9) {
            p.push_back(1);
        } else {
            p.push_back(0);
        }
    }
    return p;
}

int Model_Presence::calculateNumberOfDays(const int startDay, const int startMonth, const int endDay, const int endMonth){
    static const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int days = daysInMonth[startMonth - 1] - (startDay -1);
    for(int i = startMonth; i < endMonth -1; i++){
        days += daysInMonth[i];
    }
    days += endDay;
    return days;
}

std::vector<int> Model_Presence::calculatePresenceFromPage() {

    static const float pMon[] = {0.021, 0.021, 0.021, 0.021, 0.021, 0.021, 0.021, 0.025, 0.250, 0.422, 0.309, 0.377, 0.187, 0.375, 0.426, 0.396, 0.375, 0.432, 0.084, 0.070, 0.047, 0.039, 0.038, 0.038};
    static const float pTue[] = {0.040, 0.038, 0.038, 0.038, 0.038, 0.038, 0.038, 0.046, 0.320, 0.401, 0.325, 0.417, 0.196, 0.372, 0.435, 0.402, 0.334, 0.435, 0.100, 0.053, 0.044, 0.044, 0.042, 0.042};
    static const float pWed[] = {0.041, 0.041, 0.041, 0.041, 0.041, 0.041, 0.041, 0.062, 0.342, 0.403, 0.297, 0.342, 0.194, 0.354, 0.395, 0.363, 0.336, 0.383, 0.080, 0.043, 0.027, 0.026, 0.026, 0.026};
    static const float pThu[] = {0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.029, 0.265, 0.373, 0.271, 0.368, 0.193, 0.361, 0.402, 0.357, 0.324, 0.367, 0.094, 0.067, 0.032, 0.032, 0.030, 0.030};
    static const float pFri[] = {0.030, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.055, 0.327, 0.367, 0.304, 0.373, 0.215, 0.317, 0.280, 0.239, 0.197, 0.125, 0.066, 0.069, 0.031, 0.029, 0.027, 0.027};

    static const float pSat[] = {0.028, 0.030, 0.029, 0.029, 0.030, 0.029, 0.029, 0.030, 0.030, 0.030, 0.035, 0.037, 0.030, 0.032, 0.041, 0.045, 0.042, 0.034, 0.035, 0.032, 0.027, 0.024, 0.021, 0.021};
    static const float pSun[] = {0.021, 0.021, 0.021, 0.021, 0.021, 0.021, 0.021, 0.021, 0.024, 0.025, 0.032, 0.045, 0.040, 0.041, 0.033, 0.032, 0.033, 0.031, 0.031, 0.028, 0.026, 0.027, 0.025, 0.025};

    unsigned int timeStepsPerHour = SimulationConfig::info.timeStepsPerHour;
    unsigned int days = calculateNumberOfDays( SimulationConfig::info.startDay, SimulationConfig::info.startMonth, SimulationConfig::info.endDay, SimulationConfig::info.endMonth);
    // Model for the prediction of presence derived by J. Page
    // Reference: J. Page, D. Robinson, N. Morel, J.-L. Scartezzini, A generalised stochastic
    // model for the simulation of occupant presence, Energy and Buildings 40(2), 83-98 (2008).
    std::vector<int> occ;
    occ.push_back(false);
    double shuff = 0.11; // Mean observed value for mobility parameter
    double LongAbsCurrentDuration = 0.;

    for (unsigned int day = 1; day <= days+1; ++day) {
        // determination of the day of the week
        unsigned int dayOfTheWeek = (day - 1) % 7;
        for (unsigned int hour = 1; hour <= 24; ++hour) {
            double pHour = 0.0;
            double pNextHour = 0.0;
            if (dayOfTheWeek == 0) {
                pHour = pMon[hour - 1];
                pNextHour = ((hour == 24) ? pTue[0] : pMon[hour]);
            } else if (dayOfTheWeek == 1) {
                pHour = pTue[hour - 1];
                pNextHour = ((hour == 24) ? pWed[0] : pTue[hour]);
            } else if (dayOfTheWeek == 2) {
                pHour = pWed[hour - 1];
                pNextHour = ((hour == 24) ? pThu[0] : pWed[hour]);
            } else if (dayOfTheWeek == 3) {
                pHour = pThu[hour - 1];
                pNextHour = ((hour == 24) ? pFri[0] : pThu[hour]);
            } else if (dayOfTheWeek == 4) {
                pHour = pFri[hour - 1];
                pNextHour = ((hour == 24) ? pSat[0] : pFri[hour]);
            } else if (dayOfTheWeek == 5) {
                pHour = pSat[hour - 1];
                pNextHour = ((hour == 24) ? pSun[0] : pSat[hour]);
            } else {
                pHour = pSun[hour - 1];
                pNextHour = ((hour == 24) ? pMon[0] : pSun[hour]);
            }
            for (unsigned int fracHour = 0; fracHour < timeStepsPerHour; ++fracHour) {
                // probabilities of the current fracHour and the next fracHour
                double pcurr = ((static_cast<double>(timeStepsPerHour - fracHour)) * pHour + (static_cast<double>(fracHour)) * pNextHour) / timeStepsPerHour;
                double pnext = ((static_cast<double>(timeStepsPerHour - (fracHour + 1))) * pHour + (static_cast<double>(fracHour + 1)) * pNextHour) / timeStepsPerHour;
                double ProbLongAbsence = 0.000; // To be adjusted later, lack of calibration data.
                // --- 1. If a long absence is ongoing -----------------------
                if (LongAbsCurrentDuration > 0.) {
                    LongAbsCurrentDuration = std::max(LongAbsCurrentDuration - 1., 0.);
                    occ.push_back(false);
                }
                // --- 2. If there is no long absence ------------------------
                else {
                    // --- 2a. If a long absence starts ------------
                    if (Utility::randomDouble(0.0, 1.0) < ProbLongAbsence) {
                        occ.push_back(false);
                        LongAbsCurrentDuration = 1000.;
                    }// 1000 constante arbitraire, longueur de longue absence dans XML
                    // --- 2b. If a long absence does not start ----
                    else {
                        // Room currently not occupied
                        if (!occ.back()) {
                            if (Utility::randomDouble(0.0, 1.0) < getT01(pcurr, pnext, shuff)) {
                                occ.push_back(true);
                            } else {
                                occ.push_back(false);
                            }
                        }                            // Room currently occupied
                        else {
                            if (Utility::randomDouble(0.0, 1.0) < (1. - getT11(pcurr, pnext, shuff))) {
                                occ.push_back(false);
                            } else {
                                occ.push_back(true);
                            }
                        }
                    }
                }
            }
        }
    }

    // remove the first element of the vector (which was just for starting the process)
    occ.erase(occ.begin());

    return occ;
}


double Model_Presence::getT01(const double pcurr, const double pnext, const double shuff) {
    // This function returns the transition probabilities T01
    // pcurr: current step occupancy probability
    // pnext: next step occupancy probability
    // shuff: shuffling parameter
    // beta:  adjusted value of shuff
    double T01; // Probability to leave the space
    double beta = shuff; // default: no adjustment needed
    if (pnext == 0.) {
        T01 = 0.;
    } else if (pnext == 1.) {
        T01 = 1.;
    } else {
        if (pcurr == 1.) {
            T01 = 0.;
        } else if (pcurr == 0.) {
            T01 = pnext;
        } else if (pcurr == pnext) {
            if (pcurr + pnext > 1.) {
                if (shuff > 1. / (2.*pcurr - 1.)) {
                    beta = 1. / (2.*pcurr - 1.);
                } else {
                    beta = shuff;
                }
            } else if (pcurr + pnext < 1.) {
                if (shuff > 1. / (1. - 2.*pcurr)) {
                    beta = 1. / (1. - 2.*pcurr);
                } else {
                    beta = shuff;
                }
            } else {
                beta = shuff;
            }
            T01 = 2.*beta * pcurr / (beta + 1.);
        } else if (pcurr < pnext) {
            if (shuff < (pnext - pcurr) / (2. - (pnext + pcurr))) {
                beta = (pnext - pcurr) / (2. - (pnext + pcurr));
            } else {
                if ((pcurr + pnext > 1.) && (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                } else if ((pcurr + pnext < 1.) && (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
                    beta = (1. - pcurr + pnext) / (1. - pcurr - pnext);
                } else {
                    beta = shuff;
                }
            }
            T01 = pnext + pcurr * (beta - 1.) / (beta + 1.);
        } else { // Case of (pcurr>pnext)
            if (shuff < (pcurr - pnext) / (pnext + pcurr)) {
                beta = (pcurr - pnext) / (pnext + pcurr);
            } else {
                if ((pcurr + pnext > 1.) && (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                } else if ((pcurr + pnext < 1.) && (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
                    beta = (1. - pcurr + pnext) / (1. - pcurr - pnext);
                } else {
                    beta = shuff;
                }
            }
            T01 = pnext + pcurr * (beta - 1.) / (beta + 1.);
        }
    }
    return T01;
}

double Model_Presence::getT11(const double pcurr, const double pnext, const double shuff) {
    // This function returns the transition probabilities T01 and T11
    // pcurr: current step occupancy probability
    // pnext: next step occupancy probability
    // shuff: shuffling parameter
    // beta:  adjusted value of shuff
    double T11; // Probability to stay in the space
    if (pnext == 0.) {
        T11 = 0.;
    } else if (pnext == 1.) {
        T11 = 1.;
    } else {
        if (pcurr == 1.) {
            T11 = pnext;
        } else if (pcurr == 0.) {
            T11 = 0.;
        } else if (pcurr == pnext) {
            T11 = 1. - (1. - pcurr) * getT01(pcurr, pnext, shuff) / pcurr;
        } else { // Case of (pcurr>pnext)
            T11 = 1. / pcurr * (pnext - (1. - pcurr) * getT01(pcurr, pnext, shuff));
        }
    }
    return T11;
}
