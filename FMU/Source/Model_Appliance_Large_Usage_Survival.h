// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_SURVIVAL_H_
#define FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_SURVIVAL_H_

#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "Model_Appliance_Large_Usage.h"

class Model_Appliance_Large_Usage_Survival :
                                    public Model_Appliance_Large_Usage {
 public:
      Model_Appliance_Large_Usage_Survival();
      void setShape(double shape);
      void setScale(double scale);

      bool onAt(const int timeStep);
      void decreaseDuration();
      void setDuration(double duration);
 private:
      void parseShapeScale(rapidxml::xml_node<> *node);
      double randomWeibull() const;
      double duration;
      double shape;
      double scale;
};

#endif  // FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_SURVIVAL_H_
