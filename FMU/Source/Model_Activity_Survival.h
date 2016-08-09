// Copyright 2016 Jacob Chapman

#ifndef MODEL_ACTIVITY_SURVIVAL_H_
#define MODEL_ACTIVITY_SURVIVAL_H_

#include <vector>
#include <map>
#include "SimulationConfig.h"
#include "Model_Activity.h"

class Model_Activity_Survival : public Model_Activity {
 public:
    Model_Activity_Survival();
    int multinominalActivity(const double p[24][10], const int hourCount);
    
 private:
    void parseOther(rapidxml::xml_node<> *node);
    double randomWeibull(double scale, double shape) const;
    double duration;
    int state;
    std::map<int, std::vector<double>> shapes;
    std::map<int, std::vector<double>> scales;
};

#endif  //  MODEL_ACTIVITY_SURVIVAL_H_
