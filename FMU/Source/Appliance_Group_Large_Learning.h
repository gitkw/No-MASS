// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_LARGE_LEARNING_H_
#define APPLIANCE_GROUP_LARGE_LEARNING_H_

#include <vector>
#include "Appliance_Group.h"
#include "Appliance_Large_Learning.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */

class Appliance_Group_Large_Learning : public Appliance_Group<Appliance_Large_Learning> {
 public:
    Appliance_Group_Large_Learning();
    void setup(const std::vector<appLargeStruct> & appLarge,
                                    const int & buildingID,
                                    const std::string & buildingString);
 protected:

 private:

};

#endif  // APPLIANCE_GROUP_LARGE_LEARNING_H_
