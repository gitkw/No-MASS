/*
 * File:   VariableStore.h
 * Author: jake
 *
 * Created on September 15, 2013, 9:05 PM
 */

#ifndef VARIABLESTORE_H
#define VARIABLESTORE_H

#include <unordered_map>
#include <vector>

class VariableStore {

public:
    static void addVariable(std::string name);
    static void addValue(std::string name, double val);
    static double getValue(std::string name);
    static void print();
private:
    VariableStore();
    static std::unordered_map<std::string, std::vector<double> > variableMap;
};

#endif  /* VARIABLESTORE_H */

