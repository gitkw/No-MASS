// Copyright 2015 Jacob Chapman

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <vector>

#include "Utility.h"
#include "Model_Appliance_Usage.h"

Model_Appliance_Usage::Model_Appliance_Usage() {
        stateTv = Utility::randomInt(0.0, 1.0);
        stateWashingMachine = Utility::randomInt(0.0, 1.0);
        stateMicroWave = Utility::randomInt(0.0, 1.0);
        stateCooker = Utility::randomInt(0.0, 1.0);
        stateDishWasher = Utility::randomInt(0.0, 1.0);
        stateFridge = Utility::randomInt(0.0, 1.0);
        country = "France";
}


double Model_Appliance_Usage::consumption(const int app, const int timeStep) {
        double result = 0.0;
        if (onAt(app, timeStep)) {
                result = getPower(app);
        }
        return result;
}

double Model_Appliance_Usage::getTransitionAt(const int app,
  const int state, const int i) {
        double transition;
        switch (app) {
        case 0:
                transition = transitionsCooker[state][i];
                break;
        case 1:
                transition = transitionsMicroWave[state][i];
                break;
        case 2:
                transition = transitionsTv[state][i];
                break;
        case 3:
                transition = transitionsWashingMachine[state][i];
                break;
        case 4:
                transition = transitionsDishWasher[state][i];
                break;
        case 5:
                transition = transitionsFridge[state][i];
                break;
        }
        return transition;
}

double Model_Appliance_Usage::getPower(const int app) {
        double power = 0;
        int state;
        switch (app) {
        case 0:
                power = getMeanFraction(app) * maxPowerCooker;
                break;
        case 1:
                power = getMeanFraction(app) * maxPowerMicroWave;
                break;
        case 2:
                power = getMeanFraction(app) * maxPowerTv;
                break;
        case 3:
                power = getMeanFraction(app) * maxPowerWashingMachine;
                break;
        case 4:
                power = getMeanFraction(app) * maxPowerDishWasher;
                break;
        case 5:
                power = getMeanFraction(app) * maxPowerFridge;
                break;
        }
        return power;
}

double Model_Appliance_Usage::getMeanFraction(const int app) {
        int state;
        switch (app) {
        case 0:
                state = stateCooker;
                break;
        case 1:
                state = stateMicroWave;
                break;
        case 2:
                state = stateTv;
                break;
        case 3:
                state = stateWashingMachine;
                break;
        case 4:
                state = stateDishWasher;
                break;
        case 5:
                state = stateFridge;
                break;
        }

        double random = Utility::randomDouble(0.0, 1.0);
        double currSum = 0;
        for (int i =0; i < 10; i++) {
                currSum = currSum + getTransitionAt(app, state, i);
                if (random < currSum) {
                        state = i;
                        break;
                }
        }

        double MeanF = 0;
        switch (app) {
        case 0:
                MeanF = meanFCooker[state];
                stateCooker = state;
                break;
        case 1:
                MeanF = meanFMicrowave[state];
                stateMicroWave = state;
                break;
        case 2:
                MeanF = meanFTv[state];
                stateTv = state;
                break;
        case 3:
                MeanF = meanFWashingMachine[state];
                stateWashingMachine = state;
                break;
        case 4:
                MeanF = meanFDishWasher[state];
                stateDishWasher = state;
                break;
        case 5:
                MeanF = meanFFridge[state];
                stateFridge = state;
                break;
        }

        return MeanF;
}

bool Model_Appliance_Usage::onAt(const int app, const int timeStep) const {
        double probability = 0.0;
        switch (app) {
        case 0:
                probability = onCooker[timeStep];
                break;
        case 1:
                probability = onMicroWave[timeStep];
                break;
        case 2:
                probability = onTv[timeStep];
                break;
        case 3:
                probability = onWashingMachine[timeStep];
                break;
        case 4:
                probability = onDishWasher[timeStep];
                break;
        case 5:
                probability = onFridge[timeStep];
                break;
        }
        double random = Utility::randomDouble(0.0, 1.0);
        bool on = false;
        if (random < probability) {
                on = true;
        }
        return on;
}

template <typename T>
std::vector<T> Model_Appliance_Usage::as_vector(
  boost::property_tree::ptree::value_type const& child) {
    std::vector<T> r;
    std::vector<std::string> tokProbs;
    std::string value = child.second.get_value<std::string>();
    boost::split(tokProbs, value, boost::is_any_of(","));
    for (auto& item : tokProbs) {
        r.push_back(boost::lexical_cast<double>(item));
    }
    return r;
}

template <typename T>
std::vector<std::vector<T>> Model_Appliance_Usage::as_vector_vector(
  boost::property_tree::ptree::value_type & child) {

    std::vector<std::vector<T>> r;
    for (boost::property_tree::ptree::value_type & x: child.second) {
      r.push_back(as_vector<T>(x));
    }
    return r;
}

void Model_Appliance_Usage::parseCountryUsage(
  boost::property_tree::ptree::value_type & v) {
        for (boost::property_tree::ptree::value_type & child : v.second) {
            if (child.first == "maxPowerTv") {
                    maxPowerTv = child.second.get_value<double>();
            } else if (child.first == "maxPowerWashingMachine") {
                    maxPowerWashingMachine = child.second.get_value<double>();
            } else if (child.first == "maxPowerMicroWave") {
                    maxPowerMicroWave = child.second.get_value<double>();
            } else if (child.first == "maxPowerCooker") {
                    maxPowerCooker = child.second.get_value<double>();
            } else if (child.first == "maxPowerDishWasher") {
                    maxPowerDishWasher = child.second.get_value<double>();
            } else if (child.first == "maxPowerFridge") {
                    maxPowerFridge = child.second.get_value<double>();
            } else if (child.first == "meanFWashingMachine") {
                    meanFWashingMachine = as_vector<double>(child);
            } else if (child.first == "meanFTv") {
                    meanFTv  = as_vector<double>(child);
            } else if (child.first == "meanFCooker") {
                    meanFCooker  = as_vector<double>(child);
            } else if (child.first == "meanFMicrowave") {
                    meanFMicrowave  = as_vector<double>(child);
            } else if (child.first == "meanFFridge") {
                    meanFFridge  = as_vector<double>(child);
            } else if (child.first == "meanFDishWasher") {
                    meanFDishWasher  = as_vector<double>(child);
            } else if (child.first == "onTv"){
                    onTv = as_vector<double>(child);
            } else if (child.first == "onWashingMachine"){
                    onWashingMachine = as_vector<double>(child);
            } else if (child.first == "onMicroWave"){
                    onMicroWave = as_vector<double>(child);
            } else if (child.first == "onCooker"){
                    onCooker = as_vector<double>(child);
            } else if (child.first == "onDishWasher"){
                    onDishWasher = as_vector<double>(child);
            } else if (child.first == "onFridge"){
                    onFridge = as_vector<double>(child);
            } else if (child.first == "transitionsCooker"){
                    transitionsCooker = as_vector_vector<double>(child);
            } else if (child.first == "transitionsMicroWave"){
                    transitionsMicroWave = as_vector_vector<double>(child);
            } else if (child.first == "transitionsTv"){
                    transitionsTv = as_vector_vector<double>(child);
            } else if (child.first == "transitionsWashingMachine"){
                    transitionsWashingMachine = as_vector_vector<double>(child);
            } else if (child.first == "transitionsDishWasher"){
                    transitionsDishWasher = as_vector_vector<double>(child);
            } else if (child.first == "transitionsFridge"){
                    transitionsFridge = as_vector_vector<double>(child);
            }

        }
}

void Model_Appliance_Usage::parseConfiguration(const std::string filename) {
    namespace bpt = boost::property_tree;
    // Create an empty property tree object
    bpt::ptree pt;
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.

    bpt::read_xml(filename, pt);
    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.
    for (bpt::ptree::value_type& child : pt.get_child("Appliance")) {

        if (child.first == "Usage") {
        for (boost::property_tree::ptree::value_type & childchild : child.second) {
                if (childchild.first == "country") {
                    std::string currentCountry = childchild.second.get_value<std::string>();
                    if (currentCountry == country) {
                            parseCountryUsage(child);
                    }
                }
            }

        }

    }
}
