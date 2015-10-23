#ifndef MODEL_APPLIANCE_USAGE_H
#define	MODEL_APPLIANCE_USAGE_H

#include <boost/property_tree/ptree.hpp>

#include <vector>

class Model_Appliance_Usage {

    public:
      Model_Appliance_Usage();

      double consumption(const int app, const int timeStep);
      double getTransitionAt(const int app, const int state, const int i);
      double getPower(const int app);
      double getMeanFraction(const int app);
      bool onAt(const int app, const int timeStep) const;
      void parseConfiguration(const std::string filename);
      void setCountry(std::string name);
      std::string getCountry();

    private:
      template <typename T>
      std::vector<T> as_vector(
        boost::property_tree::ptree::value_type const& child);

      template <typename T>
      std::vector<std::vector<T>> as_vector_vector(
        boost::property_tree::ptree::value_type & child);
      void parseCountryUsage(boost::property_tree::ptree::value_type & v);

      std::string country;
      int stateTv;
      int stateWashingMachine;
      int stateMicroWave;
      int stateCooker;
      int stateDishWasher;
      int stateFridge;

      double maxPowerTv;
      double maxPowerWashingMachine;
      double maxPowerMicroWave;
      double maxPowerCooker;
      double maxPowerDishWasher;
      double maxPowerFridge;

      std::vector<double> meanFWashingMachine;
      std::vector<double> meanFTv;
      std::vector<double> meanFCooker;
      std::vector<double> meanFMicrowave;
      std::vector<double> meanFFridge;
      std::vector<double> meanFDishWasher;

      std::vector<double> onTv;
      std::vector<double> onWashingMachine;
      std::vector<double> onMicroWave;
      std::vector<double> onCooker;
      std::vector<double> onDishWasher;
      std::vector<double> onFridge;

      std::vector<std::vector<double>> transitionsCooker;
      std::vector<std::vector<double>> transitionsMicroWave;
      std::vector<std::vector<double>> transitionsTv;
      std::vector<std::vector<double>> transitionsWashingMachine;
      std::vector<std::vector<double>> transitionsDishWasher;
      std::vector<std::vector<double>> transitionsFridge;
};

#endif /* MODEL_APPLIANCE_USAGE_H */
