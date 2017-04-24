// Copyright 2016 Jacob Chapman

#ifndef MODEL_ACTIVITY_H_
#define MODEL_ACTIVITY_H_

#include <vector>
#include <map>
#include <string>
#include <cstddef>

#include "rapidxml.hpp"
#include "Model_RandomWeibull.hpp"

 /**
  * @brief Models the large appliance with survival
  * @details Models activity model for prediction of activity at each timestep adapted from
  * \n Jaboob, S. (2015). Stochastic Modelling of Occupants’ Activities and Related Behaviours. The University of Nottingham.
  */

class Model_Activity : public Model_RandomWeibull {
 public:
  Model_Activity();
  std::vector<double> preProcessActivities();
  void multinominalP(double p[4][7][24][10]) const;
  virtual int multinominalActivity(const double p[24][10], const int hourCount);

  void setAge(const std::string & age);
  void setComputer(const std::string & computer);
  void setCivstat(const std::string & civstat);
  void setUnemp(const std::string & unemp);
  void setRetired(const std::string & retired);
  void setEdtry(const std::string & edtry);
  void setFamstat(const std::string & famstat);
  void setSex(const std::string & sex);
  void setProbMap(const std::map<int, std::string> & probMap);

 protected:
  virtual void parseOther(rapidxml::xml_node<> *node);
  void parseConfiguration(const std::string filename);

  std::vector<double> disaggregate() const;
  std::vector<double> multinominal();
  std::string getSeasonString(const int month) const;
  int getSeasonInt(const int month) const;
  std::string getDay(const int day) const;
  std::map<int, std::map<std::string, std::vector<double>>> dictionary;

  std::string age;
  std::string computer;
  std::string civstat;
  std::string unemp;
  std::string retired;
  std::string edtry;
  std::string famstat;
  std::string sex;
  std::map<int, std::string> probMap;
};

#endif  // MODEL_ACTIVITY_H_
