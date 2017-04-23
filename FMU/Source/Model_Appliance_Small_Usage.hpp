// Copyright 2015 Jacob Chapman

#ifndef FMU_SOURCE_MODEL_APPLIANCE_SMALL_USAGE_H_
#define FMU_SOURCE_MODEL_APPLIANCE_SMALL_USAGE_H_

#include <string>
#include <vector>
#include "Utility.hpp"
#include "Model_RandomWeibull.hpp"

 /**
  * @brief Models small appliance
  * @details Models small appliance for prediction of appliance use at each timestep
  * \n Sancho-Tomás, A., Chapman, J., & Robinson, D. (2017). Extending No-MASS: Multi-Agent Stochastic Simulation for Demand Response of residential appliances. In Building Simulation 2017.
  */

class Model_Appliance_Small_Usage : public Model_RandomWeibull {
 public:
    Model_Appliance_Small_Usage();
    double consumption(const int timeStep);
    double weibullInvCdf(float loc, float shape, float scale) const;
    double durationAtState(int state) const;
    int calculateStateAtTenMin(int timeAsInt) const;
    void setRatedPowerAt(const int i);

    void setFolderLocation(const std::string & folderLocation);
    double getFractionalPowerAtState(int state) const;
    void readStateProbabilities(const std::string &file);
    void readWeibullParameters(const std::string &file);
    void readFractions(const std::string &file);
    void readSumRatedPowers(const std::string &file);
    Utility::uTable<double> getStateProbabilities() const;

 private:
    std::string folderLocation;
    std::string name;
    Utility::uTable<double> stateProbabilities;
    std::vector<double> fractions;
    std::vector<double> weibullLoc;
    std::vector<double> weibullShape;
    std::vector<double> weibullScale;
    std::vector<double> sumRatedPowers;

    double ratedPower;
    double duration;
    int state;
};

#endif  // FMU_SOURCE_MODEL_APPLIANCE_SMALL_USAGE_H_
