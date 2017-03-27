// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_FMI_H_
#define APPLIANCE_GROUP_FMI_H_

#include <vector>
#include "Appliance_Group.h"
#include "Appliance_FMI.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */

class Appliance_Group_FMI : public Appliance_Group<Appliance_FMI> {
 public:
    Appliance_Group_FMI();
    void setup(const std::vector<applianceStruct> & app,
                                    const int & buildingID,
                                    const std::string & buildingString);
 protected:

 private:

};

#endif  // APPLIANCE_GROUP_FMI_H_
