// Copyright 2015 Jacob Chapman

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include "Log.h"
#include "Utility.h"
#include "SimulationConfig.h"

std::vector<buildingStruct> SimulationConfig::buildings;
std::vector<agentStruct> SimulationConfig::agents;
std::map<int, windowStruct> SimulationConfig::windows;
std::map<int, shadeStruct> SimulationConfig::shades;
simulationStruct SimulationConfig::info;
int SimulationConfig::stepCount = -1;
std::string SimulationConfig::ActivityFile;
std::string SimulationConfig::FmuLocation;
std::string SimulationConfig::idfFileLocation = "in.idf";


namespace bpt = boost::property_tree;

SimulationConfig::SimulationConfig() {
}

void SimulationConfig::parseBuildings(
  bpt::ptree::value_type & v) {
    buildings.clear();
    for (bpt::ptree::value_type & child : v.second) {
      if (child.first == "building") {
          parseBuilding(child);
      }
    }
}

void SimulationConfig::parseBuilding(
  bpt::ptree::value_type & v) {
    buildingStruct b;
    b.name = buildings.size();
    for (bpt::ptree::value_type & child : v.second) {
        if (child.first == "name") {
            b.name = child.second.data();
        } else if (child.first == "agents") {
            parseAgents(child);
        } else if (child.first == "zone") {
            std::pair<std::string, ZoneStruct> zone;

            for (bpt::ptree::value_type & schild : child.second) {
                if (schild.first == "name") {
                    zone.first = schild.second.data();
                    zone.second.name = schild.second.data();
                } else if (schild.first == "activities"
                          || schild.first == "activity") {
                    zone.second.activity = schild.second.data();
                } else if (schild.first == "groundFloor") {
                    zone.second.groundFloor =
                      schild.second.get_value<bool>();
                } else if (schild.first == "windowCount") {
                    zone.second.windowCount =
                      schild.second.get_value<int>();
                }
            }

            if (zone.second.activity == "") {
                LOG << "Please define activities for zone: ";
                LOG << zone.second.name << "\n";
                LOG << "This can be done manually in the No-MASS simulation ";
                LOG << "configuration file\nOr through DesignBuilders";
                LOG << " detailed occupant interface\n";
                LOG.error();
                zone.second.activity = "NONE";
            }
            b.zones.insert(zone);
        }
    }
    buildings.push_back(b);
}

void SimulationConfig::parseAgents(bpt::ptree::value_type & v) {
    agents.clear();
    SimulationConfig::ActivityFile = "";
    info.presencePage = false;
    for (bpt::ptree::value_type & child : v.second) {
        if (child.first == "agent") {
            agentStruct agent;
            for (bpt::ptree::value_type & schild : child.second) {
                if (schild.first == "profile") {
                    for (bpt::ptree::value_type & sschild : schild.second) {
                        std::string text = sschild.first;
                        if (text == "file") {
                            SimulationConfig::ActivityFile =
                              sschild.second.get_value<std::string>();
                        } else {
                            std::pair<int, std::string> a;
                            if (text == "monday") {
                                info.presencePage = true;
                                a.first = 0;
                            } else if (text == "tuesday") {
                                a.first = 1;
                            } else  if (text == "wednesday") {
                                a.first = 2;
                            } else  if (text == "thursday") {
                                a.first = 3;
                            } else  if (text == "friday") {
                                a.first = 4;
                            } else  if (text == "saturday") {
                                a.first = 5;
                            } else  if (text == "sunday") {
                                a.first = 6;
                            } else {
                                text.erase(0, 1);
                                a.first = boost::lexical_cast<int>(text);
                            }
                            a.second = sschild.second.get_value<std::string>();
                            agent.profile.insert(a);
                        }
                    }
                } else if (schild.first == "bedroom") {
                    agent.bedroom = schild.second.data();
                } else if (schild.first == "office") {
                    agent.office = schild.second.data();
                } else if (schild.first == "power") {
                    agent.power = schild.second.get_value<double>();
                } else if (schild.first == "window") {
                    agent.windowId = schild.second.get_value<int>();
                } else if (schild.first == "shade") {
                    agent.shadeId = schild.second.get_value<int>();
                } else if (schild.first == "office") {
                    agent.office = schild.second.data();
                } else if (schild.first == "edtry") {
                    agent.edtry = schild.second.data();
                } else if (schild.first == "age") {
                    agent.age = schild.second.data();
                } else if (schild.first == "computer") {
                    agent.computer = schild.second.data();
                } else if (schild.first == "civstat") {
                    agent.civstat = schild.second.data();
                } else if (schild.first == "unemp") {
                    agent.unemp = schild.second.data();
                } else if (schild.first == "retired") {
                    agent.retired = schild.second.data();
                } else if (schild.first == "sex") {
                    agent.sex = schild.second.data();
                } else if (schild.first == "famstat") {
                    agent.famstat = schild.second.data();
                }
            }

            if (SimulationConfig::info.presencePage
                && agent.profile.size() != 7) {
                LOG << "Occupant presence has not been defined using the Page ";
                LOG << "method\nPlease add a presence profile for each day of ";
                LOG << "the week in the No-MASS simulation configuration file";
                LOG << "\nIn DesignBuilder please select the correct activity ";
                LOG << "profile\n";
                LOG.error();
            } else if (!SimulationConfig::info.presencePage
                && agent.profile.size() != 24
                && SimulationConfig::ActivityFile == "") {
                LOG << "The activity profile is not defined for each hour ";
                LOG << "using the Said method\nPlease add a activity profile ";
                LOG << "for hour in the No-MASS simulation configuration file";
                LOG << "\nIn DesignBuilder please select the correct activity ";
                LOG << "profile\n";
                LOG.error();
            }
            agents.push_back(agent);
        }
    }
}

void SimulationConfig::parseModels(bpt::ptree::value_type & v) {
    for (bpt::ptree::value_type & child : v.second) {
        if (child.first == "windows") {
            parseWindows(child);
        } else if (child.first == "shades") {
            parseShades(child);
        } else if (child.first == "lights") {
            SimulationConfig::info.lights = child.second.get_value<bool>();
        }
    }
}

void SimulationConfig::parseWindows(bpt::ptree::value_type & v) {
    windows.clear();
    for (bpt::ptree::value_type & child : v.second) {
        if (child.first == "enabled") {
            SimulationConfig::info.windows = child.second.get_value<bool>();
        } else if (child.first == "window") {
            std::pair<int, windowStruct> ws;

            for (bpt::ptree::value_type & schild : child.second) {
                if (schild.first == "id") {
                    ws.first = schild.second.get_value<int>();
                } else if (schild.first == "aop") {
                    ws.second.aop = schild.second.get_value<double>();
                } else if (schild.first == "bopout") {
                    ws.second.bopout = schild.second.get_value<double>();
                } else if (schild.first == "shapeop") {
                    ws.second.shapeop = schild.second.get_value<double>();
                } else if (schild.first == "a01arr") {
                    ws.second.a01arr = schild.second.get_value<double>();
                } else if (schild.first == "b01inarr") {
                    ws.second.b01inarr = schild.second.get_value<double>();
                } else if (schild.first == "b01outarr") {
                    ws.second.b01outarr = schild.second.get_value<double>();
                } else if (schild.first == "b01absprevarr") {
                    ws.second.b01absprevarr = schild.second.get_value<double>();
                } else if (schild.first == "b01rnarr") {
                    ws.second.b01rnarr = schild.second.get_value<double>();
                } else if (schild.first == "a01int") {
                    ws.second.a01int = schild.second.get_value<double>();
                } else if (schild.first == "b01inint") {
                    ws.second.b01inint = schild.second.get_value<double>();
                } else if (schild.first == "b01outint") {
                    ws.second.b01outint = schild.second.get_value<double>();
                } else if (schild.first == "b01presint") {
                    ws.second.b01presint = schild.second.get_value<double>();
                } else if (schild.first == "b01rnint") {
                    ws.second.b01rnint = schild.second.get_value<double>();
                } else if (schild.first == "a01dep") {
                    ws.second.a01dep = schild.second.get_value<double>();
                } else if (schild.first == "b01outdep") {
                    ws.second.b01outdep = schild.second.get_value<double>();
                } else if (schild.first == "b01absdep") {
                    ws.second.b01absdep = schild.second.get_value<double>();
                } else if (schild.first == "b01gddep") {
                    ws.second.b01gddep = schild.second.get_value<double>();
                } else if (schild.first == "a10dep") {
                    ws.second.a10dep = schild.second.get_value<double>();
                } else if (schild.first == "b10indep") {
                    ws.second.b10indep = schild.second.get_value<double>();
                } else if (schild.first == "b10outdep") {
                    ws.second.b10outdep = schild.second.get_value<double>();
                } else if (schild.first == "b10absdep") {
                    ws.second.b10absdep = schild.second.get_value<double>();
                } else if (schild.first == "b10gddep") {
                    ws.second.b10gddep = schild.second.get_value<double>();
                }
            }
            windows.insert(ws);
        }
    }
}

void SimulationConfig::parseShades(bpt::ptree::value_type & v) {
    shades.clear();
    for (bpt::ptree::value_type & child : v.second) {
        if (child.first == "enabled") {
            SimulationConfig::info.shading = child.second.get_value<bool>();
        } else if (child.first == "shade") {
            std::pair<int, shadeStruct> ws;

            for (bpt::ptree::value_type & schild : child.second) {
                if (schild.first == "id") {
                    ws.first = schild.second.get_value<int>();
                } else if (schild.first == "a01arr") {
                    ws.second.a01arr = schild.second.get_value<double>();
                } else if (schild.first == "b01inarr") {
                    ws.second.b01inarr = schild.second.get_value<double>();
                } else if (schild.first == "b01sarr") {
                    ws.second.b01sarr = schild.second.get_value<double>();
                } else if (schild.first == "a10arr") {
                    ws.second.a10arr = schild.second.get_value<double>();
                } else if (schild.first == "b10inarr") {
                    ws.second.b10inarr = schild.second.get_value<double>();
                } else if (schild.first == "b10sarr") {
                    ws.second.b10sarr = schild.second.get_value<double>();
                } else if (schild.first == "a01int") {
                    ws.second.a01int = schild.second.get_value<double>();
                } else if (schild.first == "b01inint") {
                    ws.second.b01inint = schild.second.get_value<double>();
                } else if (schild.first == "b01sint") {
                    ws.second.b01sint = schild.second.get_value<double>();
                } else if (schild.first == "a10int") {
                    ws.second.a10int = schild.second.get_value<double>();
                } else if (schild.first == "b10inint") {
                    ws.second.b10inint = schild.second.get_value<double>();
                } else if (schild.first == "b10sint") {
                    ws.second.b10sint = schild.second.get_value<double>();
                } else if (schild.first == "afullraise") {
                    ws.second.afullraise = schild.second.get_value<double>();
                } else if (schild.first == "boutfullraise") {
                    ws.second.boutfullraise = schild.second.get_value<double>();
                } else if (schild.first == "bsfullraise") {
                    ws.second.bsfullraise = schild.second.get_value<double>();
                } else if (schild.first == "bsfulllower") {
                    ws.second.bsfulllower = schild.second.get_value<double>();
                } else if (schild.first == "boutfulllower") {
                    ws.second.boutfulllower = schild.second.get_value<double>();
                } else if (schild.first == "afulllower") {
                    ws.second.afulllower = schild.second.get_value<double>();
                } else if (schild.first == "aSFlower") {
                    ws.second.aSFlower = schild.second.get_value<double>();
                } else if (schild.first == "bSFlower") {
                    ws.second.bSFlower = schild.second.get_value<double>();
                } else if (schild.first == "shapelower") {
                    ws.second.shapelower = schild.second.get_value<double>();
                }
            }
            shades.insert(ws);
        }
    }
}

/**
 * @brief Parses the simulation configuration file
 * @details Parse the configuration file for the all the parameteres needed in the simulation
 *
 * @param filename location of the simulation file to parse.
 */
void SimulationConfig::parseConfiguration(std::string filename) {
    // Create an empty property tree object
    bpt::ptree pt;
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.

    bpt::read_xml(filename, pt);
    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.
    stepCount = -1;
    info.windows = false;
    info.shading = false;
    info.lights = false;
    for (bpt::ptree::value_type & v : pt.get_child("simulation")) {
        if (v.first == "seed") {
            Utility::setSeed(v.second.get_value<int>());
        } else if (v.first == "endDay") {
            SimulationConfig::info.endDay = v.second.get_value<int>();
        } else if (v.first == "timeStepsPerHour") {
            SimulationConfig::info.timeStepsPerHour = v.second.get_value<int>();
        } else if (v.first == "beginMonth") {
            SimulationConfig::info.startMonth = v.second.get_value<int>();
        } else if (v.first == "endMonth") {
            SimulationConfig::info.endMonth = v.second.get_value<int>();
        } else if (v.first == "beginDay") {
            SimulationConfig::info.startDay = v.second.get_value<int>();
        } else if (v.first == "qlearn") {
            SimulationConfig::info.qlearn = v.second.get_value<int>();
        } else if (v.first == "qlearnep") {
            SimulationConfig::info.qlearnep = v.second.get_value<double>();
        } else if (v.first == "simulateAgents") {
            SimulationConfig::info.simulateAgents = v.second.get_value<bool>();
        } else if (v.first == "case") {
            SimulationConfig::info.caseOrder = v.second.get_value<int>();
        } else if (v.first == "buildings") {
            parseBuildings(v);
        } else if (v.first == "models") {
            parseModels(v);
        }
    }
    timeSteps();
    LOG << "Loaded No-MASS configuration without problems file: -";
    LOG << filename << "-\n";
}

std::string SimulationConfig::getZoneNameFromActivity(std::string activity) {
    std::string zoneName = "Out";  // Default out of zone
    bool found = false;
    for (buildingStruct b : buildings) {
      for (std::pair<std::string, ZoneStruct> it : b.zones) {
          std::vector<std::string> activities =
            splitZoneActivities(it.second.activity);
          for (std::string eachActivity : activities) {
              if (eachActivity == activity) {
                  zoneName = it.first;
                  found = true;
                  break;
              }
          }
          if (found) {
              break;
          }
      }
    }
    return zoneName;
}

std::vector<std::string> SimulationConfig::splitZoneActivities(
    std::string typeString) {
    std::vector<std::string> types;
    const char *str = typeString.c_str();
    char c = ',';
    do {
        const char *begin = str;
        while (*str != c && *str) {
            str++;
        }
        types.push_back(std::string(begin, str));
    } while (0 != *str++);
    return types;
}

std::vector<std::string> SimulationConfig::getActivities(std::string* name) {
    std::vector<std::string> activities;
    if (activeZone(name)) {
        ZoneStruct zone = getZone(name);
        activities = splitZoneActivities(zone.activity);
    }
    return activities;
}


ZoneStruct SimulationConfig::getZone(std::string* zoneName) {
  ZoneStruct zone;
  for (buildingStruct b : buildings) {
    if (b.zones.find(*zoneName) != b.zones.end()) {
        zone = b.zones.at(*zoneName);
        break;
    }
  }
  return zone;
}

bool SimulationConfig::activeZone(std::string* zoneName) {
  bool active = false;
  for (buildingStruct b : buildings) {
    if (b.zones.find(*zoneName) != b.zones.end()) {
      active = true;
      break;
    }
  }
  return active;
}

bool SimulationConfig::isZoneGroundFloor(std::string* zoneName) {
  bool floor = false;
  for (buildingStruct b : buildings) {
    if (b.zones.find(*zoneName) != b.zones.end()) {
        floor = b.zones.at(*zoneName).groundFloor;
        break;
    }
  }
  return floor;
}

int SimulationConfig::numberOfAgents() {
    return SimulationConfig::agents.size();
}

double SimulationConfig::lengthOfTimestep() {
    return 60.0 / SimulationConfig::info.timeStepsPerHour;
}

void SimulationConfig::step() {
    stepCount++;
}
int SimulationConfig::getStepCount() {
    return stepCount;
}

void SimulationConfig::timeSteps() {
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = daysInMonth[info.startMonth - 1] - (info.startDay - 1);
    for (int i = info.startMonth; i < info.endMonth -1; i++) {
            days += daysInMonth[i];
    }
    if (info.startMonth != info.endMonth) {
            days += info.endDay;
    }
    int hours = days * 24;
    SimulationConfig::info.timeSteps = hours * info.timeStepsPerHour;
}
