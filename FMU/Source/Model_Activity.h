// Copyright 2016 Jacob Chapman

#ifndef MODEL_ACTIVITY_H_
#define MODEL_ACTIVITY_H_

#include <vector>
#include <map>
#include <string>

#include "SimulationConfig.h"
#include "Model_RandomWeibull.h"

class Model_Activity : public Model_RandomWeibull {
 public:
  Model_Activity();
  std::vector<double> preProcessActivities(const int buildingID,
                                            const int agentID);
  void multinominalP(double p[4][7][24][10], const int buildingID,
                                              const int agentID) const;
  virtual int multinominalActivity(const double p[24][10], const int hourCount);

 protected:
  virtual void parseOther(rapidxml::xml_node<> *node);
  void parseConfiguration(const std::string filename);

  std::vector<double> disaggregate(const int buildingID,
                                    const int agentID) const;
  std::vector<double> multinominal(const int buildingID, const int agentID);
  std::string getSeasonString(const int month) const;
  int getSeasonInt(const int month) const;
  std::string getDay(const int day) const;
  std::map<int, std::map<std::string, std::vector<double>>> dictionary;
};

#endif  // MODEL_ACTIVITY_H_
