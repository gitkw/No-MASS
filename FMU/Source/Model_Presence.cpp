// Copyright 2016 Jacob Chapman

#include <ctime>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <algorithm>
#include "Utility.h"
#include "SimulationConfig.h"
#include "Model_Presence.h"

Model_Presence::Model_Presence() {
}

int Model_Presence::presentForFutureSteps() const {
    return presenceForFutureSteps.at(SimulationConfig::getStepCount());
}



void Model_Presence::calculatePresenceFromPage(const int buildingID, const int agentID) {
    float pMon[24];
    float pTue[24];
    float pWed[24];
    float pThu[24];
    float pFri[24];
    float pSat[24];
    float pSun[24];

    std::map<int, std::string> probMap =
                                  SimulationConfig::buildings[buildingID].agents.at(agentID).profile;
    for (int day = 0; day < 7; day++) {
        std::vector<std::string> tokProbs = Utility::splitCSV(probMap.at(day));

        int hour = 0;
        for (std::string strProb : tokProbs) {
            switch (day) {
              case 0:
                pMon[hour] = std::stod(strProb);
                break;
              case 1:
                pTue[hour] = std::stod(strProb);
                break;
              case 2:
                pWed[hour] = std::stod(strProb);
                break;
              case 3:
                pThu[hour] = std::stod(strProb);
                break;
              case 4:
                pFri[hour] = std::stod(strProb);
                break;
              case 5:
                pSat[hour] = std::stod(strProb);
                break;
              case 6:
                pSun[hour] = std::stod(strProb);
                break;
            }
            hour++;
        }
    }

    double timeStepsPerHour =
            static_cast<double>(SimulationConfig::info.timeStepsPerHour);
    unsigned int days = Utility::calculateNumberOfDays(SimulationConfig::info.startDay,
                                              SimulationConfig::info.startMonth,
                                              SimulationConfig::info.endDay,
                                              SimulationConfig::info.endMonth);
// Model for the prediction of presence derived by J. Page
// Reference: J. Page, D. Robinson, N. Morel, J.-L. Scartezzini,
// A generalised stochasticmodel for the simulation of occupant presence,
// Energy and Buildings 40(2), 83-98 (2008).
    std::vector<int> occ;
    occ.push_back(false);
    double shuff = 0.11;  //  Mean observed value for mobility parameter
    double LongAbsCurrentDuration = 0.0;

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
            for (int fracHour = 0; fracHour < timeStepsPerHour; ++fracHour) {
                double fracHourD = static_cast<double>(fracHour);
                // probabilities of the current fracHour and the next fracHour
                double pcurr = ((timeStepsPerHour - fracHourD) * pHour +
                                fracHourD * pNextHour) / timeStepsPerHour;
                double pnext = ((timeStepsPerHour - (fracHourD + 1)) * pHour +
                                (fracHourD + 1) * pNextHour) / timeStepsPerHour;
                // To be adjusted later, lack of calibration data.
                double ProbLongAbsence = 0.000;
                if (LongAbsCurrentDuration > 0.0) {
                    // --- 1. If a long absence is ongoing ---------------------
                    LongAbsCurrentDuration =
                                std::max(LongAbsCurrentDuration - 1.0, 0.0);
                    occ.push_back(false);
                } else {
                    // --- 2. If there is no long absence ----------------------
                    if (Utility::randomDouble(0.0, 1.0) < ProbLongAbsence) {
                        // --- 2a. If a long absence starts ------------
                        occ.push_back(false);
                        // 1000 constante arbitraire,
                        // longueur de longue absence dans XML
                        LongAbsCurrentDuration = 1000.0;
                    } else {
                        // --- 2b. If a long absence does not start ----
                        if (!occ.back()) {
                            // Room currently not occupied
                            if (Utility::randomDouble(0.0, 1.0)
                                                < getT01(pcurr, pnext, shuff)) {
                                occ.push_back(true);
                            } else {
                                occ.push_back(false);
                            }
                        } else {
                          // Room currently occupied
                            if (Utility::randomDouble(0.0, 1.0)
                                        < (1.0 - getT11(pcurr, pnext, shuff))) {
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

    // remove the first element of the vector
    // (which was just for starting the process)
    occ.erase(occ.begin());
    presenceState = occ;
}

bool Model_Presence::at(const int i) const {
    return presenceState.at(i);
}

unsigned int Model_Presence::size() const {
    return presenceState.size();
}

double Model_Presence::getT01(const double pcurr, const double pnext,
                              const double shuff) {
    // This function returns the transition probabilities T01
    // pcurr: current step occupancy probability
    // pnext: next step occupancy probability
    // shuff: shuffling parameter
    // beta:  adjusted value of shuff
    double T01;  // Probability to leave the space
    double beta = shuff;  // default: no adjustment needed
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
                if ((pcurr + pnext > 1.) &&
                      (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                } else if ((pcurr + pnext < 1.) &&
                      (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
                    beta = (1. - pcurr + pnext) / (1. - pcurr - pnext);
                } else {
                    beta = shuff;
                }
            }
            T01 = pnext + pcurr * (beta - 1.) / (beta + 1.);
        } else {  // Case of (pcurr>pnext)
            if (shuff < (pcurr - pnext) / (pnext + pcurr)) {
                beta = (pcurr - pnext) / (pnext + pcurr);
            } else {
                if ((pcurr + pnext > 1.) &&
                      (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                } else if ((pcurr + pnext < 1.) &&
                      (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
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

double Model_Presence::getT11(const double pcurr, const double pnext,
                              const double shuff) {
  // This function returns the transition probabilities T01 and T11
  // pcurr: current step occupancy probability
  // pnext: next step occupancy probability
  // shuff: shuffling parameter
  // beta:  adjusted value of shuff
  double T11;  // Probability to stay in the space
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
    } else {  // Case of (pcurr>pnext)
        T11 = 1. / pcurr * (pnext - (1. - pcurr) * getT01(pcurr, pnext, shuff));
    }
  }
  return T11;
}
