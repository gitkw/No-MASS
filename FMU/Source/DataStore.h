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

class DataStore {

public:
    static void addVariable(const std::string &name);
    static void addValue(const std::string &name, const double val);
    static double getValue(const std::string &name);
    static double getValueForZone(const std::string &name, const std::string &zoneName);
    static void print();
    static void clear();
private:
    DataStore();
    static std::unordered_map<std::string, std::vector<double> > variableMap;
};

#endif  /* VARIABLESTORE_H */
