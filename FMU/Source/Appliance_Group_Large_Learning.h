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
template <class T>
class Appliance_Group_Large_Learning : public Appliance_Group<T> {
 public:
    Appliance_Group_Large_Learning(){};

    void setup(const std::vector<applianceStruct> & app,
                                    const int & buildingID,
                                    const std::string & buildingString) {

        for (const applianceStruct &s : app) {
          this->appliances.push_back(T());
          this->appliances.back().setEpsilon(s.epsilon);
          this->appliances.back().setAlpha(s.alpha);
          this->appliances.back().setGamma(s.gamma);
          this->appliances.back().setUpdate(s.update);
          this->appliances.back().setID(s.id);
          this->appliances.back().setHoulyPriority(s.priority);
          this->appliances.back().setHoulyTimeRequired(s.timeRequired);
          this->appliances.back().setBuildingID(buildingID);
          this->appliances.back().setActivities(s.activities);
          this->appliances.back().setFile(s.fileProfile);
          this->appliances.back().setup();
          this->appliances.back().setIDString(buildingString + std::to_string(s.id));
          this->appliances.back().setupSave();
        }
        this->idString = buildingString + "_LargeLearning_Sum";
        this->setupSave();
    }
 protected:

 private:

};

#endif  // APPLIANCE_GROUP_LARGE_LEARNING_H_
