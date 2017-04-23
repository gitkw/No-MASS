// Copyright 2016 Jacob Chapman

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "rapidxml.hpp"

struct ConfigStructLVNNode {
    int id;
    double impedance = 0.0047;
    int parent;
    std::vector<int> children;
};

struct ConfigStructZone {
    std::string name;
    std::vector<int> activities;
    bool groundFloor = 0;
    bool active = false;
    int windowCount = 0;
    int id = 0;
};

struct ConfigStructAgent {
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

struct ConfigStructShade {
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

struct ConfigStructWindow {
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

struct ConfigStructAppliance{
  std::string name;
  int id;
  std::vector<double> priority;
  std::vector<double> timeRequired
  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  double cost;
  std::vector<double> costVector;

  double epsilon = 0.1;   // probability of a random action selection
  double alpha = 0.3;     // learning rate
  double gamma = 0.1;     // discount factor (how soon do you care)
  bool update = false;

  std::vector<int> activities;

  std::string fileSupply;
  std::string fileDemand;
  std::string fileProfile;

  std::string WeibullParameters;
  std::string StateProbabilities;
  std::string Fractions;
  std::string SumRatedPowers;

  std::string variableName;

  bool batteryNeighbourhoodDischarge = false;
  bool batteryNeighbourhoodCharge = false;

};

struct ConfigStructBuilding {
    std::map<std::string, ConfigStructZone> zones;
    std::vector<ConfigStructAppliance> AppliancesLarge;
    std::vector<ConfigStructAppliance> AppliancesLargeCSV;
    std::vector<ConfigStructAppliance> AppliancesGrid;
    std::vector<ConfigStructAppliance> AppliancesLargeLearning;
    std::vector<ConfigStructAppliance> AppliancesLargeLearningCSV;
    std::vector<ConfigStructAppliance> AppliancesSmall;
    std::vector<ConfigStructAppliance> AppliancesCSV;
    std::vector<ConfigStructAppliance> AppliancesFMI;
    std::vector<ConfigStructAppliance> AppliancesBattery;
    std::vector<ConfigStructAppliance> AppliancesBatteryGrid;
    std::vector<ConfigStructAgent> agents;
    std::string name;
    int id;
};

struct ConfigStructSimulation {
    bool save = false;
    bool windows = false;
    bool windowsLearn = false;
    bool lights = false;
    bool shading = false;
    bool heating = false;
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
    int startDayOfWeek = 1;
    int endDay;
    int endMonth;
    int caseOrder;
    bool ShadeClosedDuringNight;
    int precision = 5;
};

/**
* @brief The simulation configuration reader
* @details reads the configuration files and stores the values for later retrieval
*/

class Configuration {
 public:
    static void parseConfiguration(const std::string &filename);
    static void reset();
    static void step();
    static void setStepCount(const int stepcount);
    static bool isZoneGroundFloor(std::string* zoneName);
    static int getStepCount();
    static double lengthOfTimestep();
    static ConfigStructZone getZone(std::string* zoneName);

    static std::vector<ConfigStructLVNNode> lvn;
    static std::map<int, ConfigStructWindow> windows;
    static std::map<int, ConfigStructShade> shades;
    static ConfigStructSimulation info;
    static std::string FileActivity;
    static std::string FileLargeAppliance;
    static std::string FolderSmallAppliance;
    static std::string RunLocation;
    static std::vector<ConfigStructBuilding> buildings;
    static std::vector<std::string> outputRegexs;

 private:
    Configuration();
    static void timeSteps();
    static void parseBuilding(rapidxml::xml_node<> *node, const int id);
    static void parseBuildings(rapidxml::xml_node<> *node);
    static void parseLVN(rapidxml::xml_node<> *node);
    static void parseLVNNode(rapidxml::xml_node<> *node, ConfigStructLVNNode * p);
    static void parseOccupants(rapidxml::xml_node<> *node, ConfigStructBuilding *b);
    static void parseAppliances(rapidxml::xml_node<> *node, ConfigStructBuilding *b);
    static void parseModels(rapidxml::xml_node<> *node);
    static void parseWindows(rapidxml::xml_node<> *node);
    static void parseShades(rapidxml::xml_node<> *node);
    static bool strComp(const char * str1, const char * str2);
    static bool nodeNameIs(const rapidxml::xml_node<> *node, const char * str2);
    static bool nodeNameIs(const std::string & name, const char * str2);

    static std::vector<double> prioritiesToVector(
                                        const std::string & priorities);
    static std::vector<int> activityNamesToIds(
                                  const std::vector<std::string> & activities);
    static const std::string nameToLower(const rapidxml::xml_node<> *node);

    static void setValFromNodeIfName(bool * val,
                                  const std::string & value,
                                  const std::string & name,
                                  const char * expected);
    static void setValFromNodeIfName(int * val,
                                  const std::string & value,
                                  const std::string & name,
                                  const char * expected);
    static void setValFromNodeIfName(double * val,
                                  const std::string & value,
                                  const std::string & name,
                                  const char * expected);
    static void setValFromNodeIfName(float * val,
                                  const std::string & value,
                                  const std::string & name,
                                  const char * expected);

    static void setValFromNodeIfName(std::string * val,
                                  const std::string & value,
                                  const std::string & name,
                                  const char * expected);

    static int stepCount;
};

#endif  //  CONFIGURATION_H_
