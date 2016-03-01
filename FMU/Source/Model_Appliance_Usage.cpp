// Copyright 2015 Jacob Chapman


#include <cstring>
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
}

void Model_Appliance_Usage::setCountry(std::string name) {
  country = name;
}

std::string Model_Appliance_Usage::getCountry() {
  return country;
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
std::vector<T> Model_Appliance_Usage::as_vector(rapidxml::xml_node<> *node) {
    std::vector<T> r;
    std::string value = node->value();

    std::vector<std::string> tokens = Utility::splitCSV(node->value());
    for (std::string& item : tokens) {
        r.push_back(std::stod(item));
    }
    return r;
}

template <typename T>
std::vector<std::vector<T>> Model_Appliance_Usage::as_vector_vector(
  rapidxml::xml_node<> *node) {
    std::vector<std::vector<T>> r;
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
      r.push_back(as_vector<T>(cnode));
      cnode = cnode->next_sibling();
    }
    return r;
}

void Model_Appliance_Usage::parseCountryUsage(rapidxml::xml_node<> *node) {
  rapidxml::xml_node<> *cnode = node->next_sibling();
  while (cnode) {
    if (std::strcmp(cnode->name(), "maxPowerTv") == 0) {
      maxPowerTv = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "maxPowerWashingMachine") == 0) {
            maxPowerWashingMachine = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "maxPowerMicroWave") == 0) {
            maxPowerMicroWave = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "maxPowerCooker") == 0) {
            maxPowerCooker = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "maxPowerDishWasher") == 0) {
            maxPowerDishWasher = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "maxPowerFridge") == 0) {
            maxPowerFridge = std::stod(cnode->value());
    } else if (std::strcmp(cnode->name(), "meanFWashingMachine") == 0) {
            meanFWashingMachine = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "meanFTv") == 0) {
            meanFTv  = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "meanFCooker") == 0) {
            meanFCooker  = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "meanFMicrowave") == 0) {
            meanFMicrowave  = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "meanFFridge") == 0) {
            meanFFridge  = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "meanFDishWasher") == 0) {
            meanFDishWasher  = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onTv") == 0) {
            onTv = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onWashingMachine") == 0) {
            onWashingMachine = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onMicroWave") == 0) {
            onMicroWave = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onCooker") == 0) {
            onCooker = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onDishWasher") == 0) {
            onDishWasher = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "onFridge") == 0) {
            onFridge = as_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsCooker") == 0) {
            transitionsCooker = as_vector_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsMicroWave") == 0) {
            transitionsMicroWave = as_vector_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsTv") == 0) {
            transitionsTv = as_vector_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsWashingMachine") == 0) {
            transitionsWashingMachine = as_vector_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsDishWasher") == 0) {
            transitionsDishWasher = as_vector_vector<double>(cnode);
    } else if (std::strcmp(cnode->name(), "transitionsFridge") == 0) {
            transitionsFridge = as_vector_vector<double>(cnode);
    }
    cnode = cnode->next_sibling();
  }
}

void Model_Appliance_Usage::parseConfiguration(const std::string filename) {
  namespace rx = rapidxml;
  rx::file<> xmlFile(filename.c_str());  // Default template is char
  rx::xml_document<> doc;
  doc.parse<0>(xmlFile.data());    // 0 means default parse flags
  rx::xml_node<> *root_node = doc.first_node("Appliance");
  rx::xml_node<> *node = root_node->first_node("Usage");
  while (node) {
    rx::xml_node<> *cnode = node->first_node();
    while (cnode) {
      if (std::strcmp(cnode->name(), "country") == 0) {
          std::string currentCountry = cnode->value();
          if (std::strcmp(cnode->value(), country.c_str()) == 0) {
            parseCountryUsage(cnode);
          }
      }
      cnode = cnode->next_sibling();
    }
    node = node->next_sibling();
  }
}
