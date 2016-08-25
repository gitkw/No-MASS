// Copyright 2016 Jacob Chapman

#ifndef SIMULATIONCONFIG_H_
#define SIMULATIONCONFIG_H_

#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "rapidxml.hpp"

struct LVNNodeStruct {
    int id;
    int parent;
    std::vector<int> children;
};

struct ZoneStruct {
    std::string name;
    std::vector<int> activities;
    bool groundFloor = 0;
    int windowCount = 0;
    int id = 0;
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

    double ShadeClosedDuringSleep = 0;
    double ShadeClosedDuringWashing = 0;
    double ShadeDuringNight = 0;
    double ShadeDuringAudioVisual = 0;

    double LightOffDuringAudioVisual = 0;
    double LightOffDuringSleep = 0;

    double WindowOpenDuringCooking = 0;
    double WindowOpenDuringWashing = 0;
    double WindowOpenDuringSleeping = 0;

    double ApplianceDuringDay = 0;
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

struct appLargeStruct {
    std::string name;
    int id;
    double priority;
    double cost;
    double epsilon = 0.1;   // probability of a random action selection
    double alpha = 0.1;     // learning rate
    double gamma = 0.1;     // discount factor (how soon do you care)
    bool update = false;
    std::vector<int> activities;
};

struct appPVStruct {
    std::string file;
    int id;
    std::vector<double> cost;
    double priority;
};

struct appSmallStruct {
  std::string WeibullParameters;
  std::string StateProbabilities;
  std::string Fractions;
  std::string SumRatedPowers;
  int id;
  double priority;
};

struct appFMIStruct {
  std::string variableName;
  int id;
  double priority;
};

struct buildingStruct {
    std::map<std::string, ZoneStruct> zones;
    std::vector<appLargeStruct> AppliancesLarge;
    std::vector<appLargeStruct> AppliancesGrid;
    std::vector<appLargeStruct> AppliancesLargeLearning;
    std::vector<appSmallStruct> AppliancesSmall;
    std::vector<appPVStruct> AppliancesPV;
    std::vector<appFMIStruct> AppliancesFMI;
    std::vector<agentStruct> agents;
    std::string name;
    int id;
};

struct simulationStruct {
    bool save = false;
    bool windows = false;
    bool windowsLearn = false;
    bool lights = false;
    bool shading = false;
    bool presencePage = false;
    bool agentHeatGains = true;
    double timeStepsPerHour;
    double learnep;
    std::vector<double> GridCost;
    int learn;
    int learnupdate;
    int timeSteps;
    int startDay;
    int startMonth;
    int startDayOfWeek;
    int endDay;
    int endMonth;
    int caseOrder;
    bool ShadeClosedDuringNight;
};

class SimulationConfig {
 public:
    static ZoneStruct getZone(std::string* zoneName);
    static void parseConfiguration(const std::string &filename);
    static void reset();
    static bool activeZone(std::string* zoneName);
    static bool isZoneGroundFloor(std::string* zoneName);
    static std::vector<LVNNodeStruct> lvn;
    static std::map<int, windowStruct> windows;
    static std::map<int, shadeStruct> shades;
    static simulationStruct info;
    static double lengthOfTimestep();
    static void step();
    static int getStepCount();

    static int stepCount;
    static std::string ActivityFile;
    static std::string RunLocation;
    static std::vector<buildingStruct> buildings;

 private:
    static void timeSteps();
    static void parseBuilding(rapidxml::xml_node<> *node, const int id);
    static void parseBuildings(rapidxml::xml_node<> *node);
    static void parseLVN(rapidxml::xml_node<> *node);
    static void parseLVNNode(rapidxml::xml_node<> *node, LVNNodeStruct * p);
    static void parseOccupants(rapidxml::xml_node<> *node, buildingStruct *b);
    static void parseAppliances(rapidxml::xml_node<> *node, buildingStruct *b);
    static void parseModels(rapidxml::xml_node<> *node);
    static void parseWindows(rapidxml::xml_node<> *node);
    static void parseShades(rapidxml::xml_node<> *node);
    static bool strComp(const char * str1, const char * str2);
    static std::vector<double> csvToDouble(const std::string & s);
    static std::vector<int> activityNamesToIds(
                                  const std::vector<std::string> & activities);
    SimulationConfig();
};

#endif  //  SIMULATIONCONFIG_H_
