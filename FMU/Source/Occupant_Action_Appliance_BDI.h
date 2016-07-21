// Copyright 2016 Jacob Chapman
#ifndef FMU_SOURCE_Occupant_ACTION_APPLIANCE_BDI_H_
#define FMU_SOURCE_Occupant_ACTION_APPLIANCE_BDI_H_

#include <vector>
#include "Occupant_Action_Appliance.h"

class Occupant_Action_Appliance_BDI : public Occupant_Action_Appliance {
 public:
    Occupant_Action_Appliance_BDI();

    void setApplianceDuringDay(double ApplianceDuringDay);
    bool doRecipe(const std::vector<double> &activities);

 private:
    double ApplianceDuringDay;
    int first;
    int last;

    void calculateFirstLastCouts(const std::vector<double> &activities);


};

#endif  // FMU_SOURCE_Occupant_ACTION_APPLIANCE_BDI_H_
