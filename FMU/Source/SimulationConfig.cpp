// Copyright 2015 Jacob Chapman

#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include <cstring>

#include <cstddef>
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
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

SimulationConfig::SimulationConfig() {
}

void SimulationConfig::reset() {
  buildings.clear();
  agents.clear();
  windows.clear();
  shades.clear();
  info = simulationStruct();
  stepCount = -1;
  ActivityFile = "";
  FmuLocation = "";
}


void SimulationConfig::parseBuildings(rapidxml::xml_node<> *node) {
    buildings.clear();
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (std::strcmp(cnode->name(), "building") == 0) {
              parseBuilding(cnode);
        }
        cnode = cnode->next_sibling();
    }
}

void SimulationConfig::parseBuilding(rapidxml::xml_node<> *node) {
    buildingStruct b;
    b.name = buildings.size();
    int zonecount = 0;
    std::pair<std::string, ZoneStruct> zsOut;
    zsOut.first = "Out";
    zsOut.second.name = "Out";
    zsOut.second.activities.push_back(9);
    zsOut.second.id = zonecount;
    b.zones.insert(zsOut);
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (std::strcmp(cnode->name(), "building") == 0) {
            b.name = cnode->value();
        } else if (std::strcmp(cnode->name(), "agents") == 0) {
            parseAgents(cnode);
        } else if (std::strcmp(cnode->name(), "zone") == 0) {
            zonecount++;
            std::pair<std::string, ZoneStruct> zone;
            zone.second.id = zonecount;
            rapidxml::xml_node<> *znode = cnode->first_node();
            while (znode) {
                if (std::strcmp(znode->name(), "name") == 0) {
                    zone.first = znode->value();
                    zone.second.name = znode->value();
                } else if (std::strcmp(znode->name(), "activities") == 0
                          || std::strcmp(znode->name(), "activity") == 0) {
                    zone.second.activities =
                          activityNamesToIds(Utility::splitCSV(znode->value()));
                } else if (std::strcmp(znode->name(), "groundFloor") == 0) {
                    zone.second.groundFloor = std::stoi(znode->value());
                } else if (std::strcmp(znode->name(), "windowCount") == 0) {
                    zone.second.windowCount = std::stoi(znode->value());
                }
                znode = znode->next_sibling();
            }

            if (zone.second.activities.empty()) {
                LOG << "Please define activities for zone: ";
                LOG << zone.second.name << "\n";
                LOG << "This can be done manually in the No-MASS simulation ";
                LOG << "configuration file\nOr through DesignBuilders";
                LOG << " detailed occupant interface\n";
                LOG.error();
            }
            b.zones.insert(zone);
        }
        cnode = cnode->next_sibling();
    }
    buildings.push_back(b);
}

std::vector<int> SimulationConfig::activityNamesToIds(
          const std::vector<std::string> & activities) {
  std::vector<int> act;
  for (const std::string & activity : activities) {
    if (activity == "Sleep") {
      act.push_back(0);
    } else if (activity == "Passive") {
      act.push_back(1);
    } else if (activity == "AudioVisual") {
      act.push_back(2);
    } else if (activity == "IT") {
      act.push_back(3);
    } else if (activity == "Cooking") {
      act.push_back(4);
    } else if (activity == "Cleaning") {
      act.push_back(5);
    } else if (activity == "Washing") {
      act.push_back(6);
    } else if (activity == "Metabolic") {
      act.push_back(7);
    } else if (activity == "WashingAppliance") {
      act.push_back(8);
    }
  }
  return act;
}

void SimulationConfig::parseAgents(rapidxml::xml_node<> *node) {
  agents.clear();
  SimulationConfig::ActivityFile = "";
  info.presencePage = false;
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
    if (std::strcmp(cnode->name(), "agent") == 0) {
      agentStruct agent;
      rapidxml::xml_node<> *anode = cnode->first_node();
      while (anode) {
        if (std::strcmp(anode->name(), "profile") == 0) {
          rapidxml::xml_node<> *pnode = anode->first_node();
          while (pnode) {
            std::string text = pnode->name();
            if (text.compare("file") == 0) {
              SimulationConfig::ActivityFile = pnode->value();
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
                a.first = std::stoi(text);
              }
              a.second = pnode->value();
              agent.profile.insert(a);
            }
            pnode = pnode->next_sibling();
          }

        } else if (std::strcmp(anode->name(), "bedroom") == 0) {
          agent.bedroom = anode->value();
        } else if (std::strcmp(anode->name(), "office") == 0) {
          agent.office = anode->value();
        } else if (std::strcmp(anode->name(), "power") == 0) {
          agent.power = std::stod(anode->value());
        } else if (std::strcmp(anode->name(), "window") == 0) {
          agent.windowId = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(), "shade") == 0) {
          agent.shadeId = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(), "edtry") == 0) {
          agent.edtry = anode->value();
        } else if (std::strcmp(anode->name(), "age") == 0) {
          agent.age = anode->value();
        } else if (std::strcmp(anode->name(), "computer") == 0) {
          agent.computer = anode->value();
        } else if (std::strcmp(anode->name(), "civstat") == 0) {
          agent.civstat = anode->value();
        } else if (std::strcmp(anode->name(), "unemp") == 0) {
          agent.unemp = anode->value();
        } else if (std::strcmp(anode->name(), "retired") == 0) {
          agent.retired = anode->value();
        } else if (std::strcmp(anode->name(), "sex") == 0) {
          agent.sex = anode->value();
        } else if (std::strcmp(anode->name(), "famstat") == 0) {
          agent.famstat = anode->value();
        } else if (std::strcmp(anode->name(), "ShadeClosedDuringSleep") == 0) {
          agent.ShadeClosedDuringSleep = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(),
                                "ShadeClosedDuringWashing") == 0) {
          agent.ShadeClosedDuringWashing = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(),
                                "LightOffDuringAudioVisual") == 0) {
          agent.LightOffDuringAudioVisual = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(), "LightOffDuringSleep") == 0) {
          agent.LightOffDuringSleep = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(), "WindowOpenDuringCooking") == 0) {
          agent.WindowOpenDuringCooking = std::stoi(anode->value());
        } else if (std::strcmp(anode->name(), "WindowOpenDuringWashing") == 0) {
          agent.WindowOpenDuringWashing = std::stoi(anode->value());
        }
        anode = anode->next_sibling();
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
    cnode = cnode->next_sibling();
  }
}

void SimulationConfig::parseModels(rapidxml::xml_node<> *node) {
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (std::strcmp(cnode->name(), "lights") == 0) {
              SimulationConfig::info.lights = std::stoi(cnode->value());
        } else if (std::strcmp(cnode->name(), "windows") == 0) {
            parseWindows(cnode);
        } else if (std::strcmp(cnode->name(), "shades") == 0) {
            parseShades(cnode);
        }
        cnode = cnode->next_sibling();
    }
}

void SimulationConfig::parseWindows(rapidxml::xml_node<> *node) {
  windows.clear();
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
      if (std::strcmp(cnode->name(), "enabled") == 0) {
            SimulationConfig::info.windows = std::stoi(cnode->value());
      } else if (std::strcmp(cnode->name(), "window") == 0) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, windowStruct> ws;
        while (snode) {
          if (std::strcmp(snode->name(), "id") == 0) {
            ws.first = std::stoi(snode->value());
          } else if (std::strcmp(snode->name(), "aop") == 0) {
              ws.second.aop = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "bopout") == 0) {
              ws.second.bopout = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "shapeop") == 0) {
              ws.second.shapeop = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a01arr") == 0) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01inarr") == 0) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01outarr") == 0) {
              ws.second.b01outarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01absprevarr") == 0) {
              ws.second.b01absprevarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01rnarr") == 0) {
              ws.second.b01rnarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a01int") == 0) {
              ws.second.a01int = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01inint") == 0) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01outint") == 0) {
              ws.second.b01outint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01presint") == 0) {
              ws.second.b01presint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01rnint") == 0) {
              ws.second.b01rnint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a01dep") == 0) {
              ws.second.a01dep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01outdep") == 0) {
              ws.second.b01outdep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01absdep") == 0) {
              ws.second.b01absdep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01gddep") == 0) {
              ws.second.b01gddep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a10dep") == 0) {
              ws.second.a10dep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10indep") == 0) {
              ws.second.b10indep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10outdep") == 0) {
              ws.second.b10outdep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10absdep") == 0) {
              ws.second.b10absdep = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10gddep") == 0) {
              ws.second.b10gddep = std::stod(snode->value());
          }
          snode = snode->next_sibling();
        }

        windows.insert(ws);
      }
      cnode = cnode->next_sibling();
  }
}


void SimulationConfig::parseShades(rapidxml::xml_node<> *node) {
  shades.clear();
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
      if (std::strcmp(cnode->name(), "enabled") == 0) {
            SimulationConfig::info.shading = std::stoi(cnode->value());
      } else if (std::strcmp(cnode->name(), "shade") == 0) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, shadeStruct> ws;
        while (snode) {
          if (std::strcmp(snode->name(), "id") == 0) {
            ws.first = std::stoi(snode->value());
          } else if (std::strcmp(snode->name(), "a01arr") == 0) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01inarr") == 0) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01sarr") == 0) {
              ws.second.b01sarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a10arr") == 0) {
              ws.second.a10arr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10inarr") == 0) {
              ws.second.b10inarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10sarr") == 0) {
              ws.second.b10sarr = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a01int") == 0) {
              ws.second.a01int = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01inint") == 0) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b01sint") == 0) {
              ws.second.b01sint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "a10int") == 0) {
              ws.second.a10int = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10inint") == 0) {
              ws.second.b10inint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "b10sint") == 0) {
              ws.second.b10sint = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "afullraise") == 0) {
              ws.second.afullraise = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "boutfullraise") == 0) {
              ws.second.boutfullraise = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "bsfullraise") == 0) {
              ws.second.bsfullraise = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "bsfulllower") == 0) {
              ws.second.bsfulllower = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "boutfulllower") == 0) {
              ws.second.boutfulllower = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "afulllower") == 0) {
              ws.second.afulllower = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "aSFlower") == 0) {
              ws.second.aSFlower = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "bSFlower") == 0) {
              ws.second.bSFlower = std::stod(snode->value());
          } else if (std::strcmp(snode->name(), "shapelower") == 0) {
              ws.second.shapelower = std::stod(snode->value());
          }
          snode = snode->next_sibling();
        }
        shades.insert(ws);
      }
      cnode = cnode->next_sibling();
  }
}

/**
 * @brief Parses the simulation configuration file
 * @details Parse the configuration file for the all the parameteres needed in the simulation
 *
 * @param filename location of the simulation file to parse.
 */

void SimulationConfig::parseConfiguration(const std::string & filename) {
  namespace rx = rapidxml;
  rx::file<> xmlFile(filename.c_str());  // Default template is char
  rx::xml_document<> doc;
  doc.parse<0>(xmlFile.data());    // 0 means default parse flags
  rx::xml_node<> *root_node = doc.first_node("simulation");
  rx::xml_node<> *node = root_node->first_node();
  while (node) {
    if (std::strcmp(node->name(), "seed") == 0) {
        Utility::setSeed(std::stoi(node->value()));
    } else if (std::strcmp(node->name(), "endDay") == 0) {
        SimulationConfig::info.endDay = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "timeStepsPerHour") == 0) {
        SimulationConfig::info.timeStepsPerHour = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "beginMonth") == 0) {
        SimulationConfig::info.startMonth = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "endMonth") == 0) {
        SimulationConfig::info.endMonth = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "beginDay") == 0) {
        SimulationConfig::info.startDay = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "learn") == 0) {
        SimulationConfig::info.learn = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "learnep") == 0) {
        SimulationConfig::info.learnep = std::stod(node->value());
    } else if (std::strcmp(node->name(), "case") == 0) {
        SimulationConfig::info.caseOrder = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "ShadeClosedDuringNight") == 0) {
       SimulationConfig::info.ShadeClosedDuringNight
           = std::stoi(node->value());
    } else if (std::strcmp(node->name(), "models") == 0) {
       parseModels(node);
    } else if (std::strcmp(node->name(), "buildings") == 0) {
        parseBuildings(node);
    }

    node = node->next_sibling();
  }
  timeSteps();
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
  if (*zoneName != "Out") {
    for (buildingStruct b : buildings) {
      if (b.zones.find(*zoneName) != b.zones.end()) {
        active = true;
        break;
      }
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
    return 3600 / SimulationConfig::info.timeStepsPerHour;
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
