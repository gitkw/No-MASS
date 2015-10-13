/*
 * File:   VariableStore.cpp
 * Author: jake
 *
 * Created on September 15, 2013, 9:05 PM
 */

#include <vector>
#include <iostream>
#include <fstream>
#include "Log.h"
#include "DataStore.h"

std::unordered_map<std::string, std::vector<double> > DataStore::variableMap;

DataStore::DataStore() {
}

void DataStore::addVariable(std::string name) {
    variableMap.insert(std::make_pair(name, std::vector<double>()));
}

void DataStore::addValue(std::string name, double value) {
    variableMap[name].push_back(value);
}

double DataStore::getValueForZone(std::string name, std::string zoneName){
    return getValue(zoneName + name);
}

double DataStore::getValue(std::string name) {
    if (variableMap.find(name) == variableMap.end()) {
        LOG << "Forgot to define: " << name;
        LOG << "Check the Zone Name is correct in the NoMass Simulation File";
        LOG.error();
        return 0;
    }

    return variableMap[name].back();
}

void DataStore::clear(){
    variableMap.clear();
}

void DataStore::print(){
/*    std::ofstream myfile;
    myfile.open ("agent.csv");
    myfile << "stepCount,";
    for (std::unordered_map<std::string, std::vector<double> >::iterator it=variableMap.begin(); it!=variableMap.end(); ++it){
        myfile << it->first << ",";
    }
    myfile << std::endl;
    for(unsigned int i =0; i < variableMap.begin()->second.size(); i++ ){
        myfile << i << ",";
        for (std::unordered_map<std::string, std::vector<double> >::iterator it=variableMap.begin(); it!=variableMap.end(); ++it){
            if(it->second.size() > i){
                myfile << it->second.at(i);
            }
            myfile << ",";
        }
        myfile << std::endl;
    }
    myfile.close();
    */
}
