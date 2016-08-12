// Copyright 2016 Jacob Chapman

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
std::map<int, windowStruct> SimulationConfig::windows;
std::map<int, shadeStruct> SimulationConfig::shades;
simulationStruct SimulationConfig::info;
int SimulationConfig::stepCount = -1;
std::string SimulationConfig::ActivityFile;
std::string SimulationConfig::RunLocation;

SimulationConfig::SimulationConfig() {}

void SimulationConfig::reset() {
  buildings.clear();
  windows.clear();
  shades.clear();
  info = simulationStruct();
  stepCount = -1;
  ActivityFile = "";
  RunLocation = "";
}


void SimulationConfig::parseBuildings(rapidxml::xml_node<> *node) {
    buildings.clear();
    rapidxml::xml_node<> *cnode = node->first_node();
    int id = 0;
    while (cnode) {
        if (strComp(cnode->name(), "building")) {
              parseBuilding(cnode, id);
              id++;
        }
        cnode = cnode->next_sibling();
    }
}

void SimulationConfig::parseBuilding(rapidxml::xml_node<> *node, const int id) {
    buildingStruct b;
    b.id = id;
    int zonecount = 0;
    std::pair<std::string, ZoneStruct> zsOut;
    zsOut.first = "Out";
    zsOut.second.name = "Out";
    zsOut.second.activities.push_back(9);
    zsOut.second.id = zonecount;
    b.zones.insert(zsOut);
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (strComp(cnode->name(), "name")) {
            b.name = cnode->value();
        } else if (strComp(cnode->name(), "agents")) {
            parseOccupants(cnode, &b);
        } else if (strComp(cnode->name(), "Appliances")) {
            parseAppliances(cnode, &b);
        } else if (strComp(cnode->name(), "zone")) {
            zonecount++;
            std::pair<std::string, ZoneStruct> zone;
            zone.second.id = zonecount;
            rapidxml::xml_node<> *znode = cnode->first_node();
            while (znode) {
                if (strComp(znode->name(), "name")) {
                    zone.first = znode->value();
                    zone.second.name = znode->value();
                } else if (strComp(znode->name(), "activities")
                          || strComp(znode->name(), "activity")) {
                    zone.second.activities =
                          activityNamesToIds(Utility::splitCSV(znode->value()));
                } else if (strComp(znode->name(), "groundFloor")) {
                    zone.second.groundFloor = std::stoi(znode->value());
                } else if (strComp(znode->name(), "windowCount")) {
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

void SimulationConfig::parseAppliances(rapidxml::xml_node<> *node,
                                                            buildingStruct *b) {
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
    if (strComp(cnode->name(), "Large") ||
                strComp(cnode->name(), "LargeLearning") ||
                            strComp(cnode->name(), "Grid")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appLargeStruct s;
      while (anode) {
        if (strComp(anode->name(), "id")) {
          s.id = std::stoi(anode->value());
        } else if (strComp(anode->name(), "priority")) {
          s.priority = std::stoi(anode->value());
        } else if (strComp(anode->name(), "activities")) {
          s.activities = activityNamesToIds(Utility::splitCSV(anode->value()));
        }
        anode = anode->next_sibling();
      }
      if (strComp(cnode->name(), "Large")) {
        b->AppliancesLarge.push_back(s);
      } else if (strComp(cnode->name(), "LargeLearning")) {
        b->AppliancesLargeLearning.push_back(s);
      } else if (strComp(cnode->name(), "Grid")) {
        b->AppliancesGrid.push_back(s);
      }
    } else if (strComp(cnode->name(), "Small")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appSmallStruct s;
      while (anode) {
        if (strComp(anode->name(), "WeibullParameters")) {
          s.WeibullParameters = anode->value();
        } else if (strComp(anode->name(), "StateProbabilities")) {
          s.StateProbabilities = anode->value();
        } else if (strComp(anode->name(), "Fractions")) {
          s.Fractions = anode->value();
        } else if (strComp(anode->name(), "SumRatedPowers")) {
          s.SumRatedPowers = anode->value();
        } else if (strComp(anode->name(), "id")) {
          s.id = std::stoi(anode->value());
        } else if (strComp(anode->name(), "priority")) {
          s.priority = std::stoi(anode->value());
        }
        anode = anode->next_sibling();
      }
      b->AppliancesSmall.push_back(s);
    }  else if (strComp(cnode->name(), "FMI")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appFMIStruct s;
      while (anode) {
        if (strComp(anode->name(), "variablename")) {
          s.variableName = anode->value();
        } else if (strComp(anode->name(), "id")) {
          s.id = std::stoi(anode->value());
        } else if (strComp(anode->name(), "priority")) {
          s.priority = std::stoi(anode->value());
        }
        anode = anode->next_sibling();
      }
      b->AppliancesFMI.push_back(s);
    } else if (strComp(cnode->name(), "pv")) {
        rapidxml::xml_node<> *anode = cnode->first_node();
        appPVStruct s;
        while (anode) {
          if (strComp(anode->name(), "filename")) {
            s.file = anode->value();
          } else if (strComp(anode->name(), "id")) {
            s.id = std::stoi(anode->value());
          } else if (strComp(anode->name(), "priority")) {
            s.priority = std::stoi(anode->value());
          }
          anode = anode->next_sibling();
        }
        b->AppliancesPV.push_back(s);
      }
    cnode = cnode->next_sibling();
  }
}

void SimulationConfig::parseOccupants(rapidxml::xml_node<> *node,
                                                            buildingStruct *b) {
  SimulationConfig::ActivityFile = "";
  info.presencePage = false;
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
    if (strComp(cnode->name(), "agent")) {
      agentStruct agent;
      rapidxml::xml_node<> *anode = cnode->first_node();
      while (anode) {
        if (strComp(anode->name(), "profile")) {
          rapidxml::xml_node<> *pnode = anode->first_node();
          while (pnode) {
            std::string text = pnode->name();
            if (text == "file") {
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
        } else if (strComp(anode->name(), "bedroom")) {
          agent.bedroom = anode->value();
        } else if (strComp(anode->name(), "office")) {
          agent.office = anode->value();
        } else if (strComp(anode->name(), "power")) {
          agent.power = std::stod(anode->value());
        } else if (strComp(anode->name(), "window")) {
          agent.windowId = std::stoi(anode->value());
        } else if (strComp(anode->name(), "shade")) {
          agent.shadeId = std::stoi(anode->value());
        } else if (strComp(anode->name(), "edtry")) {
          agent.edtry = anode->value();
        } else if (strComp(anode->name(), "age")) {
          agent.age = anode->value();
        } else if (strComp(anode->name(), "computer")) {
          agent.computer = anode->value();
        } else if (strComp(anode->name(), "civstat")) {
          agent.civstat = anode->value();
        } else if (strComp(anode->name(), "unemp")) {
          agent.unemp = anode->value();
        } else if (strComp(anode->name(), "retired")) {
          agent.retired = anode->value();
        } else if (strComp(anode->name(), "sex")) {
          agent.sex = anode->value();
        } else if (strComp(anode->name(), "famstat")) {
          agent.famstat = anode->value();
        } else if (strComp(anode->name(), "ShadeClosedDuringSleep")) {
          agent.ShadeClosedDuringSleep = std::stod(anode->value());
        } else if (strComp(anode->name(), "ShadeClosedDuringWashing")) {
          agent.ShadeClosedDuringWashing = std::stod(anode->value());
        } else if (strComp(anode->name(), "ShadeDuringNight")) {
          agent.ShadeDuringNight = std::stod(anode->value());
        } else if (strComp(anode->name(), "ShadeDuringAudioVisual")) {
          agent.ShadeDuringAudioVisual = std::stod(anode->value());
        } else if (strComp(anode->name(), "LightOffDuringAudioVisual")) {
          agent.LightOffDuringAudioVisual = std::stod(anode->value());
        } else if (strComp(anode->name(), "LightOffDuringSleep")) {
          agent.LightOffDuringSleep = std::stod(anode->value());
        } else if (strComp(anode->name(), "WindowOpenDuringCooking")) {
          agent.WindowOpenDuringCooking = std::stod(anode->value());
        } else if (strComp(anode->name(), "WindowOpenDuringWashing")) {
          agent.WindowOpenDuringWashing = std::stod(anode->value());
        } else if (strComp(anode->name(), "WindowOpenDuringSleeping")) {
          agent.WindowOpenDuringSleeping = std::stod(anode->value());
        } else if (strComp(anode->name(), "ApplianceDuringDay")) {
          agent.ApplianceDuringDay = std::stod(anode->value());
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
      b->agents.push_back(agent);
    }
    cnode = cnode->next_sibling();
  }
}

void SimulationConfig::parseModels(rapidxml::xml_node<> *node) {
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (strComp(cnode->name(), "AgentHeatGains")) {
            SimulationConfig::info.agentHeatGains = std::stoi(cnode->value());
        } else if (strComp(cnode->name(), "lights")) {
            SimulationConfig::info.lights = std::stoi(cnode->value());
        } else if (strComp(cnode->name(), "windows")) {
            parseWindows(cnode);
        } else if (strComp(cnode->name(), "shades")) {
            parseShades(cnode);
        }
        cnode = cnode->next_sibling();
    }
}

void SimulationConfig::parseWindows(rapidxml::xml_node<> *node) {
  windows.clear();
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
      if (strComp(cnode->name(), "windowsLearn")) {
        SimulationConfig::info.windowsLearn = std::stoi(cnode->value());
      } else if (strComp(cnode->name(), "enabled")) {
        SimulationConfig::info.windows = std::stoi(cnode->value());
      } else if (strComp(cnode->name(), "window")) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, windowStruct> ws;
        while (snode) {
          if (strComp(snode->name(), "id")) {
            ws.first = std::stoi(snode->value());
          } else if (strComp(snode->name(), "aop")) {
              ws.second.aop = std::stod(snode->value());
          } else if (strComp(snode->name(), "bopout")) {
              ws.second.bopout = std::stod(snode->value());
          } else if (strComp(snode->name(), "shapeop")) {
              ws.second.shapeop = std::stod(snode->value());
          } else if (strComp(snode->name(), "a01arr")) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01inarr")) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01outarr")) {
              ws.second.b01outarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01absprevarr")) {
              ws.second.b01absprevarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01rnarr")) {
              ws.second.b01rnarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "a01int")) {
              ws.second.a01int = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01inint")) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01outint")) {
              ws.second.b01outint = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01presint")) {
              ws.second.b01presint = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01rnint")) {
              ws.second.b01rnint = std::stod(snode->value());
          } else if (strComp(snode->name(), "a01dep")) {
              ws.second.a01dep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01outdep")) {
              ws.second.b01outdep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01absdep")) {
              ws.second.b01absdep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01gddep")) {
              ws.second.b01gddep = std::stod(snode->value());
          } else if (strComp(snode->name(), "a10dep")) {
              ws.second.a10dep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10indep")) {
              ws.second.b10indep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10outdep")) {
              ws.second.b10outdep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10absdep")) {
              ws.second.b10absdep = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10gddep")) {
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
      if (strComp(cnode->name(), "enabled")) {
            SimulationConfig::info.shading = std::stoi(cnode->value());
      } else if (strComp(cnode->name(), "shade")) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, shadeStruct> ws;
        while (snode) {
          if (strComp(snode->name(), "id")) {
            ws.first = std::stoi(snode->value());
          } else if (strComp(snode->name(), "a01arr")) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01inarr")) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01sarr")) {
              ws.second.b01sarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "a10arr")) {
              ws.second.a10arr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10inarr")) {
              ws.second.b10inarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10sarr")) {
              ws.second.b10sarr = std::stod(snode->value());
          } else if (strComp(snode->name(), "a01int")) {
              ws.second.a01int = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01inint")) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (strComp(snode->name(), "b01sint")) {
              ws.second.b01sint = std::stod(snode->value());
          } else if (strComp(snode->name(), "a10int")) {
              ws.second.a10int = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10inint")) {
              ws.second.b10inint = std::stod(snode->value());
          } else if (strComp(snode->name(), "b10sint")) {
              ws.second.b10sint = std::stod(snode->value());
          } else if (strComp(snode->name(), "afullraise")) {
              ws.second.afullraise = std::stod(snode->value());
          } else if (strComp(snode->name(), "boutfullraise")) {
              ws.second.boutfullraise = std::stod(snode->value());
          } else if (strComp(snode->name(), "bsfullraise")) {
              ws.second.bsfullraise = std::stod(snode->value());
          } else if (strComp(snode->name(), "bsfulllower")) {
              ws.second.bsfulllower = std::stod(snode->value());
          } else if (strComp(snode->name(), "boutfulllower")) {
              ws.second.boutfulllower = std::stod(snode->value());
          } else if (strComp(snode->name(), "afulllower")) {
              ws.second.afulllower = std::stod(snode->value());
          } else if (strComp(snode->name(), "aSFlower")) {
              ws.second.aSFlower = std::stod(snode->value());
          } else if (strComp(snode->name(), "bSFlower")) {
              ws.second.bSFlower = std::stod(snode->value());
          } else if (strComp(snode->name(), "shapelower")) {
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
    if (strComp(node->name(), "seed")) {
        Utility::setSeed(std::stoi(node->value()));
    } else if (strComp(node->name(), "save")) {
        SimulationConfig::info.save = std::stoi(node->value());
    } else if (strComp(node->name(), "endDay")) {
        SimulationConfig::info.endDay = std::stoi(node->value());
    } else if (strComp(node->name(), "timeStepsPerHour")) {
        SimulationConfig::info.timeStepsPerHour = std::stoi(node->value());
    } else if (strComp(node->name(), "beginMonth")) {
        SimulationConfig::info.startMonth = std::stoi(node->value());
    } else if (strComp(node->name(), "endMonth")) {
        SimulationConfig::info.endMonth = std::stoi(node->value());
    } else if (strComp(node->name(), "beginDay")) {
        SimulationConfig::info.startDay = std::stoi(node->value());
    } else if (strComp(node->name(), "learn")) {
        SimulationConfig::info.learn = std::stoi(node->value());
    } else if (strComp(node->name(), "learnupdate")) {
        SimulationConfig::info.learnupdate = std::stoi(node->value());
    } else if (strComp(node->name(), "learnep")) {
        SimulationConfig::info.learnep = std::stod(node->value());
    } else if (strComp(node->name(), "case")) {
        SimulationConfig::info.caseOrder = std::stoi(node->value());
    } else if (strComp(node->name(), "ShadeClosedDuringNight")) {
       SimulationConfig::info.ShadeClosedDuringNight
           = std::stoi(node->value());
    } else if (strComp(node->name(), "models")) {
       parseModels(node);
    } else if (strComp(node->name(), "buildings")) {
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


bool SimulationConfig::strComp(const char * str1, const char * str2) {
  return std::strcmp(str1, str2) == 0;
}
