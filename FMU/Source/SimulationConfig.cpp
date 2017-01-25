// Copyright 2016 Jacob Chapman

#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <functional>
#include <cstring>

#include <cstddef>
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "Log.h"
#include "Utility.h"
#include "SimulationConfig.h"

std::vector<buildingStruct> SimulationConfig::buildings;
std::vector<std::string> SimulationConfig::outputRegexs;
std::vector<LVNNodeStruct> SimulationConfig::lvn;
std::map<int, windowStruct> SimulationConfig::windows;
std::map<int, shadeStruct> SimulationConfig::shades;
simulationStruct SimulationConfig::info;
int SimulationConfig::stepCount = -1;
std::string SimulationConfig::FileActivity;
std::string SimulationConfig::FileLargeAppliance;
std::string SimulationConfig::FolderSmallAppliance;
std::string SimulationConfig::RunLocation;

SimulationConfig::SimulationConfig() {}

void SimulationConfig::reset() {
  buildings.clear();
  windows.clear();
  shades.clear();
  outputRegexs.clear();
  info = simulationStruct();
  stepCount = -1;
  FileActivity = "";
  FileLargeAppliance = "";
  FolderSmallAppliance = "";
  RunLocation = "";
}

void SimulationConfig::parseLVN(rapidxml::xml_node<> *node) {
    buildings.clear();
    rapidxml::xml_node<> *cnode = node->first_node();
    LVNNodeStruct l;
    l.id = -1;
    parseLVNNode(cnode, &l);
}

void SimulationConfig::parseLVNNode(rapidxml::xml_node<> *node,
                                                        LVNNodeStruct * p) {
    rapidxml::xml_node<> *cnode = node->first_node();
    bool children = false;
    LVNNodeStruct l;
    l.parent = p->id;
    while (cnode) {
        if (nodeNameIs(cnode, "children")) {
          children = true;
        } else if (nodeNameIs(cnode, "id")) {
          l.id = std::stoi(cnode->value());
        } else if (nodeNameIs(cnode, "impedance")) {
          l.impedance = std::stod(cnode->value());
        }
        cnode = cnode->next_sibling();
    }
    if (children) {
      cnode = node->first_node();
      while (cnode) {
          if (nodeNameIs(cnode, "children")) {
            parseLVNNode(cnode->first_node(), &l);
          }
          cnode = cnode->next_sibling();
      }
    }
    p->children.push_back(l.id);
    lvn.push_back(l);
}

void SimulationConfig::parseBuildings(rapidxml::xml_node<> *node) {
    buildings.clear();
    rapidxml::xml_node<> *cnode = node->first_node();
    int id = 0;
    while (cnode) {
        if (nodeNameIs(cnode, "building")) {
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
    zsOut.first = "out";
    zsOut.second.name = "out";
    zsOut.second.activities.push_back(9);
    zsOut.second.id = zonecount;
    zsOut.second.active = false;
    b.zones.insert(zsOut);
    rapidxml::xml_node<> *cnode = node->first_node();
    while (cnode) {
        if (nodeNameIs(cnode, "name")) {
            b.name = cnode->value();
        } else if (nodeNameIs(cnode, "agents")) {
            parseOccupants(cnode, &b);
        } else if (nodeNameIs(cnode, "id")) {
            b.id = std::stoi(cnode->value());
        } else if (nodeNameIs(cnode, "appliances")) {
            parseAppliances(cnode, &b);
        } else if (nodeNameIs(cnode, "zone")) {
            zonecount++;
            std::pair<std::string, ZoneStruct> zone;
            zone.second.id = zonecount;
            zone.second.active = true;
            rapidxml::xml_node<> *znode = cnode->first_node();
            while (znode) {
                if (nodeNameIs(znode, "name")) {
                    zone.first = znode->value();
                    zone.second.name = znode->value();
                } else if (nodeNameIs(znode, "activities")
                          || nodeNameIs(znode, "activity")) {
                    zone.second.activities =
                          activityNamesToIds(Utility::splitCSV(znode->value()));
                } else if (nodeNameIs(znode, "groundfloor")) {
                    zone.second.groundFloor = std::stoi(znode->value());
                } else if (nodeNameIs(znode, "windowcount")) {
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

std::vector<double> SimulationConfig::prioritiesToVector(
                            const std::string & priorities) {
  std::vector<double> prior(csvToDouble(priorities));
  while (prior.size() < 24) {
    prior.push_back(prior.back());
  }
  return prior;
}

void SimulationConfig::parseAppliances(rapidxml::xml_node<> *node,
                                                            buildingStruct *b) {
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
    if (nodeNameIs(cnode, "large") ||
                nodeNameIs(cnode, "largelearning") ||
                            nodeNameIs(cnode, "grid")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appLargeStruct s;
      while (anode) {
        if (nodeNameIs(anode, "id")) {
          s.id = std::stoi(anode->value());
        } else if (nodeNameIs(anode, "priority")) {
          s.priority = prioritiesToVector(anode->value());
        } else if (nodeNameIs(anode, "timerequired")) {
          s.timeRequired = prioritiesToVector(anode->value());
        } else if (nodeNameIs(anode, "activities")) {
          s.activities = activityNamesToIds(Utility::splitCSV(anode->value()));
        } else if (nodeNameIs(anode, "cost")) {
          s.cost = std::stod(anode->value());
        } else if (nodeNameIs(anode, "epsilon")) {
          s.epsilon = std::stod(anode->value());
        } else if (nodeNameIs(anode, "alpha")) {
          s.alpha = std::stod(anode->value());
        } else if (nodeNameIs(anode, "gamma")) {
          s.gamma = std::stod(anode->value());
        } else if (nodeNameIs(anode, "updateqtable")) {
          s.update = std::stod(anode->value());
        }
        anode = anode->next_sibling();
      }
      if (nodeNameIs(cnode, "large")) {
        b->AppliancesLarge.push_back(s);
      } else if (nodeNameIs(cnode, "largelearning")) {
        b->AppliancesLargeLearning.push_back(s);
      } else if (nodeNameIs(cnode, "grid")) {
        b->AppliancesGrid.push_back(s);
      }
    } else if (nodeNameIs(cnode, "small")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appSmallStruct s;
      while (anode) {
        if (nodeNameIs(anode, "weibullparameters")) {
          s.WeibullParameters = anode->value();
        } else if (nodeNameIs(anode, "stateprobabilities")) {
          s.StateProbabilities = anode->value();
        } else if (nodeNameIs(anode, "fractions")) {
          s.Fractions = anode->value();
        } else if (nodeNameIs(anode, "sumratedpowers")) {
          s.SumRatedPowers = anode->value();
        } else if (nodeNameIs(anode, "id")) {
          s.id = std::stoi(anode->value());
        } else if (nodeNameIs(anode, "priority")) {
          s.priority = prioritiesToVector(anode->value());
        }
        anode = anode->next_sibling();
      }
      b->AppliancesSmall.push_back(s);
    }  else if (nodeNameIs(cnode, "fmi")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appFMIStruct s;
      while (anode) {
        if (nodeNameIs(anode, "variablename")) {
          s.variableName = anode->value();
        } else if (nodeNameIs(anode, "id")) {
          s.id = std::stoi(anode->value());
        } else if (nodeNameIs(anode, "priority")) {
          s.priority = prioritiesToVector(anode->value());
        }
        anode = anode->next_sibling();
      }
      b->AppliancesFMI.push_back(s);
    }  else if (nodeNameIs(cnode, "battery") && nodeNameIs(cnode, "batteryGridCostReward")) {
      rapidxml::xml_node<> *anode = cnode->first_node();
      appBatteryStruct s;
      while (anode) {
        if (nodeNameIs(anode, "id")) {
          s.id = std::stoi(anode->value());
        } else if (nodeNameIs(anode, "priority")) {
          s.priority = prioritiesToVector(anode->value());
        } else if (nodeNameIs(anode, "epsilon")) {
          s.epsilon = std::stod(anode->value());
        } else if (nodeNameIs(anode, "alpha")) {
          s.alpha = std::stod(anode->value());
        } else if (nodeNameIs(anode, "gamma")) {
          s.gamma = std::stod(anode->value());
        } else if (nodeNameIs(anode, "updateqtable")) {
          s.update = std::stod(anode->value());
        }
        anode = anode->next_sibling();
      }
      if (nodeNameIs(cnode, "battery")) {
        b->AppliancesBattery.push_back(s);
      } else {
        b->AppliancesBatteryGrid.push_back(s);
      }
    } else if (nodeNameIs(cnode, "pv") || nodeNameIs(cnode, "csv")) {
        rapidxml::xml_node<> *anode = cnode->first_node();
        appCSVStruct s;
        while (anode) {
          if (nodeNameIs(anode, "filename") || nodeNameIs(anode, "supply")) {
            s.fileSupply = anode->value();
          } else if (nodeNameIs(anode, "demand")) {
            s.fileDemand = anode->value();
          } else if (nodeNameIs(anode, "id")) {
            s.id = std::stoi(anode->value());
          } else if (nodeNameIs(anode, "priority")) {
            s.priority = prioritiesToVector(anode->value());
          } else if (nodeNameIs(anode, "cost")) {
            s.cost = csvToDouble(anode->value());
          }
          anode = anode->next_sibling();
        }
        b->AppliancesCSV.push_back(s);
      }
    cnode = cnode->next_sibling();
  }
}

void SimulationConfig::parseOccupants(rapidxml::xml_node<> *node,
                                                            buildingStruct *b) {
  info.presencePage = false;
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
    if (nodeNameIs(cnode, "agent")) {
      agentStruct agent;
      rapidxml::xml_node<> *anode = cnode->first_node();
      while (anode) {
        const std::string aname = nameToLower(anode);
        if (nodeNameIs(aname, "profile")) {
          rapidxml::xml_node<> *pnode = anode->first_node();
          while (pnode) {
            std::string text = pnode->name();
            if (text == "file") {
              SimulationConfig::FileActivity = SimulationConfig::RunLocation
                                                + pnode->value();
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
        } else if (nodeNameIs(aname, "bedroom")) {
          agent.bedroom = anode->value();
        } else if (nodeNameIs(aname, "office")) {
          agent.office = anode->value();
        } else if (nodeNameIs(aname, "power")) {
          agent.power = std::stod(anode->value());
        } else if (nodeNameIs(aname, "window")) {
          agent.windowId = std::stoi(anode->value());
        } else if (nodeNameIs(aname, "shade")) {
          agent.shadeId = std::stoi(anode->value());
        } else if (nodeNameIs(aname, "edtry")) {
          agent.edtry = anode->value();
        } else if (nodeNameIs(aname, "age")) {
          agent.age = anode->value();
        } else if (nodeNameIs(aname, "computer")) {
          agent.computer = anode->value();
        } else if (nodeNameIs(aname, "civstat")) {
          agent.civstat = anode->value();
        } else if (nodeNameIs(aname, "unemp")) {
          agent.unemp = anode->value();
        } else if (nodeNameIs(aname, "retired")) {
          agent.retired = anode->value();
        } else if (nodeNameIs(aname, "sex")) {
          agent.sex = anode->value();
        } else if (nodeNameIs(aname, "famstat")) {
          agent.famstat = anode->value();
        } else if (nodeNameIs(aname, "shadeclosedduringsleep")) {
          agent.ShadeClosedDuringSleep = std::stod(anode->value());
        } else if (nodeNameIs(aname, "shadeclosedduringwashing")) {
          agent.ShadeClosedDuringWashing = std::stod(anode->value());
        } else if (nodeNameIs(aname, "shadeduringnight")) {
          agent.ShadeDuringNight = std::stod(anode->value());
        } else if (nodeNameIs(aname, "shadeduringaudiovisual")) {
          agent.ShadeDuringAudioVisual = std::stod(anode->value());
        } else if (nodeNameIs(aname, "lightoffduringaudiovisual")) {
          agent.LightOffDuringAudioVisual = std::stod(anode->value());
        } else if (nodeNameIs(aname, "lightoffduringsleep")) {
          agent.LightOffDuringSleep = std::stod(anode->value());
        } else if (nodeNameIs(aname, "windowopenduringcooking")) {
          agent.WindowOpenDuringCooking = std::stod(anode->value());
        } else if (nodeNameIs(aname, "windowopenduringwashing")) {
          agent.WindowOpenDuringWashing = std::stod(anode->value());
        } else if (nodeNameIs(aname, "windowopenduringsleeping")) {
          agent.WindowOpenDuringSleeping = std::stod(anode->value());
        } else if (nodeNameIs(aname, "applianceduringday")) {
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
        && SimulationConfig::FileActivity == "") {
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
        if (nodeNameIs(cnode, "agentheatgains")) {
            SimulationConfig::info.agentHeatGains = std::stoi(cnode->value());
        } else if (nodeNameIs(cnode, "lights")) {
            SimulationConfig::info.lights = std::stoi(cnode->value());
        } else if (nodeNameIs(cnode, "windows")) {
            parseWindows(cnode);
        } else if (nodeNameIs(cnode, "shades")) {
            parseShades(cnode);
        }
        cnode = cnode->next_sibling();
    }
}

void SimulationConfig::parseWindows(rapidxml::xml_node<> *node) {
  windows.clear();
  rapidxml::xml_node<> *cnode = node->first_node();
  while (cnode) {
      if (nodeNameIs(cnode, "windowslearn")) {
        SimulationConfig::info.windowsLearn = std::stoi(cnode->value());
      } else if (nodeNameIs(cnode, "enabled")) {
        SimulationConfig::info.windows = std::stoi(cnode->value());
      } else if (nodeNameIs(cnode, "window")) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, windowStruct> ws;
        while (snode) {
          const std::string sname = nameToLower(snode);
          if (nodeNameIs(sname, "id")) {
            ws.first = std::stoi(snode->value());
          } else if (nodeNameIs(sname, "aop")) {
              ws.second.aop = std::stod(snode->value());
          } else if (nodeNameIs(sname, "bopout")) {
              ws.second.bopout = std::stod(snode->value());
          } else if (nodeNameIs(sname, "shapeop")) {
              ws.second.shapeop = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a01arr")) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01inarr")) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01outarr")) {
              ws.second.b01outarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01absprevarr")) {
              ws.second.b01absprevarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01rnarr")) {
              ws.second.b01rnarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a01int")) {
              ws.second.a01int = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01inint")) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01outint")) {
              ws.second.b01outint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01presint")) {
              ws.second.b01presint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01rnint")) {
              ws.second.b01rnint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a01dep")) {
              ws.second.a01dep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01outdep")) {
              ws.second.b01outdep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01absdep")) {
              ws.second.b01absdep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01gddep")) {
              ws.second.b01gddep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a10dep")) {
              ws.second.a10dep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10indep")) {
              ws.second.b10indep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10outdep")) {
              ws.second.b10outdep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10absdep")) {
              ws.second.b10absdep = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10gddep")) {
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
      if (nodeNameIs(cnode, "enabled")) {
            SimulationConfig::info.shading = std::stoi(cnode->value());
      } else if (nodeNameIs(cnode, "shade")) {
        rapidxml::xml_node<> *snode = cnode->first_node();
        std::pair<int, shadeStruct> ws;
        while (snode) {
          const std::string sname = nameToLower(snode);
          if (nodeNameIs(sname, "id")) {
            ws.first = std::stoi(snode->value());
          } else if (nodeNameIs(sname, "a01arr")) {
              ws.second.a01arr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01inarr")) {
              ws.second.b01inarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01sarr")) {
              ws.second.b01sarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a10arr")) {
              ws.second.a10arr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10inarr")) {
              ws.second.b10inarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10sarr")) {
              ws.second.b10sarr = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a01int")) {
              ws.second.a01int = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01inint")) {
              ws.second.b01inint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b01sint")) {
              ws.second.b01sint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "a10int")) {
              ws.second.a10int = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10inint")) {
              ws.second.b10inint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "b10sint")) {
              ws.second.b10sint = std::stod(snode->value());
          } else if (nodeNameIs(sname, "afullraise")) {
              ws.second.afullraise = std::stod(snode->value());
          } else if (nodeNameIs(sname, "boutfullraise")) {
              ws.second.boutfullraise = std::stod(snode->value());
          } else if (nodeNameIs(sname, "bsfullraise")) {
              ws.second.bsfullraise = std::stod(snode->value());
          } else if (nodeNameIs(sname, "bsfulllower")) {
              ws.second.bsfulllower = std::stod(snode->value());
          } else if (nodeNameIs(sname, "boutfulllower")) {
              ws.second.boutfulllower = std::stod(snode->value());
          } else if (nodeNameIs(sname, "afulllower")) {
              ws.second.afulllower = std::stod(snode->value());
          } else if (nodeNameIs(sname, "asflower")) {
              ws.second.aSFlower = std::stod(snode->value());
          } else if (nodeNameIs(sname, "bsflower")) {
              ws.second.bSFlower = std::stod(snode->value());
          } else if (nodeNameIs(sname, "shapelower")) {
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
    const std::string name = nameToLower(node);
    if (nodeNameIs(name, "seed")) {
        Utility::setSeed(std::stoi(node->value()));
    } else if (nodeNameIs(name, "save")) {
        SimulationConfig::info.save = std::stoi(node->value());
    } else if (nodeNameIs(name, "endday")) {
        SimulationConfig::info.endDay = std::stoi(node->value());
    } else if (nodeNameIs(name, "timestepsperhour")) {
        SimulationConfig::info.timeStepsPerHour = std::stoi(node->value());
    } else if (nodeNameIs(name, "beginmonth")) {
        SimulationConfig::info.startMonth = std::stoi(node->value());
    } else if (nodeNameIs(name, "endmonth")) {
        SimulationConfig::info.endMonth = std::stoi(node->value());
    } else if (nodeNameIs(name, "beginday")) {
        SimulationConfig::info.startDay = std::stoi(node->value());
    } else if (nodeNameIs(name, "learn")) {
        SimulationConfig::info.learn = std::stoi(node->value());
    } else if (nodeNameIs(name, "startdayofweek")) {
        SimulationConfig::info.startDayOfWeek = std::stoi(node->value());
    } else if (nodeNameIs(name, "learnupdate")) {
        SimulationConfig::info.learnupdate = std::stoi(node->value());
    } else if (nodeNameIs(name, "learnep")) {
        SimulationConfig::info.learnep = std::stod(node->value());
    } else if (nodeNameIs(name, "gridcost")) {
        SimulationConfig::info.GridCost = csvToDouble(node->value());
    } else if (nodeNameIs(name, "case")) {
        SimulationConfig::info.caseOrder = std::stoi(node->value());
    } else if (nodeNameIs(name, "output")) {
      rapidxml::xml_node<> *cnode = node->first_node();
      while (cnode) {
          if (nodeNameIs(cnode, "regex")) {
            outputRegexs.push_back(cnode->value());
          }
          cnode = cnode->next_sibling();
      }
    } else if (nodeNameIs(name, "shadeclosedduringnight")) {
       SimulationConfig::info.ShadeClosedDuringNight
           = std::stoi(node->value());
    } else if (nodeNameIs(name, "models")) {
       parseModels(node);
    } else if (nodeNameIs(name, "buildings")) {
        parseBuildings(node);
    } else if (nodeNameIs(name, "lvn")) {
        parseLVN(node);
    }

    node = node->next_sibling();
  }

  FileLargeAppliance = RunLocation + "AppliancesLarge.xml";
  FolderSmallAppliance = RunLocation + "SmallAppliances/";

  timeSteps();
  if (outputRegexs.empty()) {
    outputRegexs.push_back(".*");
  }
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

void SimulationConfig::setStepCount(const int stepcount) {
  stepCount = stepcount;
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

const std::string SimulationConfig::nameToLower(
                                            const rapidxml::xml_node<> *node) {
    std::string name(node->name());
  for (unsigned int i = 0; i < name.size(); i++) {
    name[i] = std::tolower(name[i]);
  }
  return name;
}

bool SimulationConfig::nodeNameIs(const std::string & name, const char * str2) {
  return strComp(name.c_str(), str2);
}

bool SimulationConfig::nodeNameIs(const rapidxml::xml_node<> *node,
                                                        const char * str2) {
  return strComp(nameToLower(node).c_str(), str2);
}

bool SimulationConfig::strComp(const char * str1, const char * str2) {
  return std::strcmp(str1, str2) == 0;
}

std::vector<double> SimulationConfig::csvToDouble(const std::string & s) {
  std::vector<double> items;
  for (std::string &item : Utility::splitCSV(s)) {
      items.push_back(std::stod(item));
  }
  return items;
}
