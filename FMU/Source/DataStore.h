// Copyright 2015 Jacob Chapman

#ifndef DATASTORE_H
#define DATASTORE_H

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

#endif  /* DATASTORE_H */
