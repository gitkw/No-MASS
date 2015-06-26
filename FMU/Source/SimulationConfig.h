/*
 * File:   SimulationSetup.h
 * Author: jake
 *
 * Created on September 13, 2013, 10:17 AM
 */

#ifndef SIMULATIONSETUP_H
#define	SIMULATIONSETUP_H

#include <string>
#include <vector>
#include <map>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

struct ZoneStruct {
    std::string name;
    std::string activity;
    bool groundFloor;
};

struct agentStruct {
    std::string name;
    std::string bedroom;
    std::string office;
    double power;
    int windowId;
    std::map<int, std::string> profile;
};

struct windowStruct {
    double aop;
    double bopout;
    double shapeop;

    double a01arr;
    double b01inarr;
    double b01outarr;
    double b01absprevarr;
    double b01rnarr;
    // P01int
    double a01int;
    double b01inint;
    double b01outint;
    double b01presint;
    double b01rnint;
    // P01dep
    double a01dep;
    double b01outdep;
    double b01absdep;
    double b01gddep;
    // P10dep
    double a10dep;
    double b10indep;
    double b10outdep;
    double b10absdep;
    double b10gddep;
};

struct stateStruct {
    std::string name;
    std::string location;
    int met;
    int clo;
};

struct simulationStruct {

    bool simulateAgents;
    bool windows;
    bool lights;
    bool shading;
    int timeSteps;
    double timeStepsPerHour;
    bool presencePage;
    int startDay;
    int startMonth;
    int endDay;
    int endMonth;
    int timePeriod;
    int caseOrder;
};

class SimulationConfig {
public:
    static std::vector<std::string> getActivities(std::string* name);
    static void parseConfiguration(std::string);
    static bool activeZone(std::string* zoneName);
    static bool isZoneGroundFloor(std::string* zoneName);
    static std::string getZoneNameFromActivity(std::string activityName);
    static std::map<std::string, ZoneStruct> zones;
    static std::vector<agentStruct> agents;
    static std::map<int, windowStruct> windows;
    static std::vector<stateStruct> states;
    static simulationStruct info;
    static int numberOfAgents();
    static double lengthOfTimestep();
    static void step();
    static int getStepCount();
private:
    static int stepCount;
    static std::vector<std::string> splitZoneActivities(std::string types);
    static void parseBuilding(boost::property_tree::ptree::value_type & v);
    static void parseAgents(boost::property_tree::ptree::value_type & v);
    static void parseModels(boost::property_tree::ptree::value_type & v);
    static void parseStates(boost::property_tree::ptree::value_type & v);
    static void parseWindows(boost::property_tree::ptree::value_type & v);
    SimulationConfig();
    SimulationConfig(const SimulationConfig& orig);
    virtual ~SimulationConfig();
};

#endif	/* SIMULATIONSETUP_H */

