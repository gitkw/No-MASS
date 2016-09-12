// Copyright 2016 Jacob Chapman

#ifndef MODEL_APPLIANCE_LARGE_USAGE_H_
#define MODEL_APPLIANCE_LARGE_USAGE_H_

#include <string>
#include <vector>
#include <cstddef>
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "Model_RandomWeibull.h"

class Model_Appliance_Large_Usage : public Model_RandomWeibull {
 public:
      Model_Appliance_Large_Usage();

      double consumption(const int timeStep);
      double getPower();
      double getMeanFraction();
      virtual bool onAt(const int timeStep);
      void parseConfiguration(const std::string filename);
      void setCountry(const std::string & name);
      void setID(const int id);
      std::string getCountry();
      bool isOn() const;

 protected:
      template <typename T>
      std::vector<T> as_vector(rapidxml::xml_node<> *node);
      template <typename T>
      std::vector<std::vector<T>> as_vector_vector(rapidxml::xml_node<> *node);

      double probOn(int timestep) const;
      virtual void parseShapeScale(rapidxml::xml_node<> *node);
      bool on;
      std::string name;
      std::string country;
      int id;
      int state;
      double maxPower;
      std::vector<double> meanF;
      std::vector<double> onProbabilities;
      std::vector<std::vector<double>> transitions;
};

#endif  // MODEL_APPLIANCE_LARGE_USAGE_H_
