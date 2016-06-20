// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_AGENT_ACTION_APPLIANCE_BDI_H_
#define FMU_SOURCE_AGENT_ACTION_APPLIANCE_BDI_H_

#include <vector>
#include "Agent_Action_Appliance.h"

class Agent_Action_Appliance_BDI : public Agent_Action_Appliance {
 public:
    Agent_Action_Appliance_BDI();

    void setApplianceDuringDay(double ApplianceDuringDay);
    bool doRecipe(const std::vector<double> &activities);

 private:
    double ApplianceDuringDay;
    int first;
    int last;

    void calculateFirstLastCouts(const std::vector<double> &activities);


};

#endif  // FMU_SOURCE_AGENT_ACTION_APPLIANCE_BDI_H_
