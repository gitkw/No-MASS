// Copyright 2016 Jacob Chapman

#ifndef MODEL_PRESENCE_H_
#define MODEL_PRESENCE_H_
#include <vector>
#include <map>
#include <string>
#include "Model_Activity.hpp"

/**
 * @brief Models the Presence of an occupant
 * @details Models the Presence of an occupant adapted from
 * \n Page, J., Robinson, D., Morel, N., & Scartezzini, J.-L. (2008). A generalised stochastic model for the simulation of occupant presence. Energy and Buildings, 40(2), 83–98. https://doi.org/10.1016/j.enbuild.2007.01.018
 */

class Model_Presence {
 public:
    Model_Presence();
    void calculatePresenceFromPage();
    void setProbMap(const std::map<int, std::string> & probMap);
    unsigned int size() const;
    bool at(const int i) const;
    int presentForFutureSteps() const;
 private:
    std::vector<int> presenceForFutureSteps;
    std::vector<int> currentDurationOfPresenceState;
    std::vector<int> presenceState;
    std::map<int, std::string> probMap;
    double getT11(double pcurr, double pnext, double shuff);
    double getT01(double pcurr, double pnext, double shuff);
};

#endif  // MODEL_PRESENCE_H_
