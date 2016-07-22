// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_H_
#define FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_H_


#include <string>
#include <vector>
#include <cstddef>
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"

class Model_Appliance_Large_Usage {
 public:
      Model_Appliance_Large_Usage();

      double consumption(const int timeStep);
      double getTransitionAt(const int state, const int i);
      double getPower();
      double getMeanFraction();
      bool onAt(const int timeStep) const;
      void parseConfiguration(const std::string filename);
      void setCountry(const std::string & name);
      void setID(const int id);
      std::string getCountry();

 private:
      template <typename T>
      std::vector<T> as_vector(rapidxml::xml_node<> *node);
      template <typename T>
      std::vector<std::vector<T>> as_vector_vector(rapidxml::xml_node<> *node);
      void parseCountryUsage(rapidxml::xml_node<> *node);

      double probOn(int timestep) const;

      std::string name;
      std::string country;
      int id;
      int state;
      double maxPower;
      std::vector<double> meanF;
      std::vector<double> on;
      std::vector<std::vector<double>> transitions;
};

#endif  // FMU_SOURCE_MODEL_APPLIANCE_LARGE_USAGE_H_
