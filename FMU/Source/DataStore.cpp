// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include "Log.h"
#include "SimulationConfig.h"
#include "DataStore.h"

std::unordered_map<std::string, std::vector<double> > DataStore::variableMap;

DataStore::DataStore() {}

void DataStore::addVariable(const std::string &name) {
    if (name != "") {
        variableMap.insert(std::make_pair(name, std::vector<double>()));
    }
}

void DataStore::addValue(const std::string &name, const double value) {
    if (name != "") {
        variableMap[name].push_back(value);
    }
}

double DataStore::getValueForZone(const std::string &name,
                                        const std::string &zoneName) {
    return getValue(zoneName + name);
}

double DataStore::getValue(const std::string &name) {
  if (variableMap.find(name) == variableMap.end()) {
    LOG << "Cannot find the variable: " << name;
    LOG << "\nThis could happen for a number of reasons:\n";
    LOG << " - Check the Zone Name is correct in ";
    LOG << "the NoMass simulation configuration file\n";
    LOG << " - Check that all variable are defined ";
    LOG << "in the model description file\n";
    LOG.error();
    exit(-1);
    return 0;
  }
  return variableMap[name].back();
}

void DataStore::clear() {
    variableMap.clear();
}

void DataStore::print() {
  if (SimulationConfig::info.save) {
    std::ofstream myfile;
    myfile.open("NoMASS.out");
    myfile << "stepCount,";
    unsigned int maxSize = 0;
    std::unordered_map<std::string, std::vector<double> >::const_iterator it;
    for (it=variableMap.cbegin(); it != variableMap.cend(); ++it) {
        myfile << it->first << ",";
        if (maxSize < it->second.size()) {
          maxSize = it->second.size();
        }
    }
    myfile << "\n";
    for (unsigned int i =0; i < maxSize; i++) {
        myfile << i << ",";
        for (it=variableMap.cbegin(); it != variableMap.cend(); ++it) {
            if (it->second.size() > i) {
                myfile << static_cast<float>(it->second.at(i));
            }
            myfile << ",";
        }
        myfile << "\n";
    }
    myfile.close();
  }
}
