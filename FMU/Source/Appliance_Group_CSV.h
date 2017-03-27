// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_CSV_H_
#define APPLIANCE_GROUP_CSV_H_

#include <vector>
#include "Appliance_Group.h"
#include "Appliance_Generic_CSV.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */

class Appliance_Group_CSV : public Appliance_Group<Appliance_Generic_CSV> {
 public:
    Appliance_Group_CSV();
    void setup(const std::vector<applianceStruct> & appCSV,
                                    const int & buildingID,
                                    const std::string & buildingString);
 protected:

 private:

};

#endif  // APPLIANCE_GROUP_CSV_H_
