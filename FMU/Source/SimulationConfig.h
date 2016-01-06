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
    std::vector<std::string> activities;
    bool groundFloor;
    int windowCount;
};

struct agentStruct {
    std::string name;
    std::string bedroom;
    std::string office;
    double power;
    int windowId;
    int shadeId;
    std::map<int, std::string> profile;

    std::string age;
    std::string computer;
    std::string civstat;
    std::string unemp;
    std::string retired;
    std::string edtry;
    std::string famstat;
    std::string sex;

    bool HeatOnPresence;
    bool ShadeClosedDuringSleep;
    bool ShadeClosedDuringWashing;
    bool ShadeClosedDuringNight;
    bool LightOffDuringNight;
    bool LightOffDuringAudioVisual;
    bool LightOffDuringOut;
    bool WindowOpenDuringCooking;
    bool WindowOpenDuringWashing;

};

struct shadeStruct {
    float a01arr;
    float b01inarr;
    float b01sarr;
    // Probability of raising on arrival
    float a10arr;
    float b10inarr;
    float b10sarr;

    float a01int;
    float b01inint;
    float b01sint;
    // Probability of raising during presence
    float a10int;
    float b10inint;
    float b10sint;

    float afullraise;
    float boutfullraise;
    float bsfullraise;

    float bsfulllower;
    float boutfulllower;
    float afulllower;

    float aSFlower;
    float bSFlower;
    float shapelower;
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

struct buildingStruct {
    std::map<std::string, ZoneStruct> zones;
    std::string name;
};

struct simulationStruct {

    bool simulateAgents;
    bool windows;
    bool lights;
    bool shading;
    bool presencePage;
    double timeStepsPerHour;
    double qlearnep;
    int qlearn;
    int timeSteps;
    int startDay;
    int startMonth;
    int startDayOfWeek;
    int endDay;
    int endMonth;
    int caseOrder;
};

class SimulationConfig {
public:
    static ZoneStruct getZone(std::string* zoneName);
    static void parseConfiguration(std::string);
    static void reset();
    static bool activeZone(std::string* zoneName);
    static bool isZoneGroundFloor(std::string* zoneName);
    static std::vector<buildingStruct> buildings;
    static std::vector<agentStruct> agents;
    static std::map<int, windowStruct> windows;
    static std::map<int, shadeStruct> shades;
    static simulationStruct info;
    static int numberOfAgents();
    static double lengthOfTimestep();
    static void step();
    static int getStepCount();
    static int stepCount;
    static std::string ActivityFile;
    static std::string FmuLocation;

private:
    static void timeSteps();
    static void parseBuilding(boost::property_tree::ptree::value_type & v);
    static void parseBuildings(boost::property_tree::ptree::value_type & v);
    static void parseAgents(boost::property_tree::ptree::value_type & v);
    static void parseModels(boost::property_tree::ptree::value_type & v);
    static void parseWindows(boost::property_tree::ptree::value_type & v);
    static void parseShades(boost::property_tree::ptree::value_type & v);
    SimulationConfig();
};

#endif	/* SIMULATIONSETUP_H */
