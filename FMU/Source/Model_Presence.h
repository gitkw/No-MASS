// Copyright 2016 Jacob Chapman

#ifndef MODEL_PRESENCE_H
#define	MODEL_PRESENCE_H
#include <vector>

#include "Model_Activity.h"

class Model_Presence {
public:

    Model_Presence();
    void calculatePresenceFromPage(const int buildingID, const int agentID);
    unsigned int size() const;
    bool at(const int i) const;
    int presentForFutureSteps() const;
private:

    std::vector<int> presenceForFutureSteps;
    std::vector<int> currentDurationOfPresenceState;
    std::vector<int> presenceState;
    double getT11(double pcurr, double pnext, double shuff);
    double getT01(double pcurr, double pnext, double shuff);
};

#endif	/* MODEL_PRESENCE_H */
