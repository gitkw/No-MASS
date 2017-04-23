// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_SURVIVAL_H_
#define FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_SURVIVAL_H_

#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "Model_Appliance_Large_Usage.hpp"

 /**
  * @brief Models the large appliance with survival
  * @details Models large appliance model for prediction of appliance use at each timestep
   * with applaince survival in on state
  * \n Jaboob, S. (2015). Stochastic Modelling of Occupants’ Activities and Related Behaviours. The University of Nottingham.
  */

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
