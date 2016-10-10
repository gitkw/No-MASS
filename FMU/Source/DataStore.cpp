// Copyright 2015 Jacob Chapman

#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include "Log.h"
#include "SimulationConfig.h"
#include "DataStore.h"

std::unordered_map<std::string, int> DataStore::variableMap;
std::unordered_map<int, std::vector<float>> DataStore::intMap;

int DataStore::variableCount = 0;

DataStore::DataStore() {}

int DataStore::getID(const std::string &name) {
  //  std::cout << "getID: " << name << std::endl;
  return variableMap.at(name);
}

int DataStore::addVariable(const std::string &name) {
  int ret = -1;
  if (name != "") {
    if (variableMap.find(name) == variableMap.end()) {
      //  std::cout << "addVariable: " << ret << ": " << name << std::endl;
      ret = DataStore::variableCount;
      //  std::cout << "addVariable: " << ret << ": " << name << std::endl;
      variableMap.insert(std::make_pair(name, ret));
      intMap.insert(std::make_pair(ret, std::vector<float>()));
      DataStore::variableCount++;
    } else {
      ret = getID(name);
    }
  }
  return ret;
}

void DataStore::addValueS(const std::string &name, const double value) {
    if (name != "") {
        //  std::cout << "addValueS: " << name << ": " << value << std::endl;
        int val = variableMap.at(name);
        //  std::cout << "addValueS: " << val;
        //  std::cout << ": " << name << ": " << value << std::endl;
        addValue(val, value);
    }
}

void DataStore::addValue(const int & id, const double val) {
  intMap.at(id).push_back(val);
}


double DataStore::getValueForZone(const std::string &name,
                                        const std::string &zoneName) {
    return getValueS(zoneName + name);
}

double DataStore::getValueS(const std::string & name) {
  //  std::cout << "get: " << name << std::endl;
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
  int id = variableMap.at(name);
  //  std::cout << "get: " << id << ": " << name << std::endl;
  return getValue(id);
}

double DataStore::getValue(const int & id) {
  //  std::cout << "getValue: " << id << ": ";
  double ret = intMap.at(id).back();
  //  std::cout << ret << std::endl;

  return ret;
}

void DataStore::clear() {
    variableMap.clear();
    intMap.clear();
}

void DataStore::print() {
  if (SimulationConfig::info.save) {
    //  std::cout << "print " << std::endl;
    std::ofstream myfile;
    myfile.open("NoMASS.out");
    myfile << "stepCount,";
    std::vector<int> ids;
    unsigned int maxSize = 0;
    std::unordered_map<std::string, int >::const_iterator it;
    for (it=variableMap.cbegin(); it != variableMap.cend(); ++it) {
        myfile << it->first << ",";
        int val = it->second;
        ids.push_back(val);
        if (maxSize < intMap.at(val).size()) {
          maxSize = intMap.at(val).size();
        }
    }
    myfile << "\n";

    for (unsigned int i =0; i < maxSize; i++) {
        myfile << i << ",";
        for (int j : ids) {
            if (intMap.at(j).size() > i) {
                myfile << intMap.at(j).at(i);
            }
            myfile << ",";
        }
        myfile << "\n";
    }
    myfile.close();
  }
}
