/*
 * File:   VariableStore.cpp
 * Author: jake
 *
 * Created on September 15, 2013, 9:05 PM
 */

#include <vector>
#include <iostream>
#include <fstream>
#include "VariableStore.h"

std::unordered_map<std::string, std::vector<double> > VariableStore::variableMap;

VariableStore::VariableStore() {
}

void VariableStore::addVariable(std::string name) {
    variableMap[name] = std::vector<double>();
}

void VariableStore::addValue(std::string name, double value) {
    variableMap[name].push_back(value);
}

double VariableStore::getValue(std::string name) {
    if (variableMap.find(name) == variableMap.end()) {
        std::cout << "Forgot to define: " << name << std::endl;
        std::cout << "Check the Zone Name is correct in the NoMass Simulation File" << std::endl;
        std::cout << std::flush;
        exit(-1);
    }
    return variableMap[name].back();
}

void VariableStore::print(){
    std::ofstream myfile;
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
}
