// Copyright 2015 Jacob Chapman

#include <map>
#include <limits>
#include <utility>
#include <vector>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstddef>
#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"

#include "SimulationConfig.h"
#include "Utility.h"
#include "Model_Activity.h"

/**
 * sleep
 * passive
 * Audio / visul
 * IT
 * Cooking
 * Cleaning
 * Other Washing
 * Metabolic
 * Washing applicance
 * out
 */

Model_Activity::Model_Activity() {}

std::vector<double> Model_Activity::preProcessActivities() {
    if (SimulationConfig::FileActivity == "") {
        return disaggregate();
    } else {
        parseConfiguration(SimulationConfig::FileActivity);
        return multinominal();
    }
}


std::string Model_Activity::getSeasonString(const int month) const {
  std::string season;
  switch (month) {
    case 0:
      season = "season1";
      break;
    case 1:
      season = "season2";
      break;
    case 2:
      season = "season3";
      break;
    default:
      season = "season4";
  }
  return season;
}

int Model_Activity::getSeasonInt(const int month) const {
  int season;
  switch (month) {
    case 12:
    case 1:
    case 2:
      season = 3;
      break;
    case 3:
    case 4:
    case 5:
      season = 0;
      break;
    case 6:
    case 7:
    case 8:
      season = 1;
    break;
    default:
      season = 2;
  }
  return season;
}

std::string Model_Activity::getDay(const int day) const {
  return "day" + std::to_string(day);
}

int Model_Activity::multinominalActivity(const double p[24][10],
                                                          const int hourCount) {
  return Utility::cumulativeProbability(p[hourCount], 10);
}

void Model_Activity::multinominalP(double p[4][7][24][10]) const {

    for (int iSeason = 0; iSeason <4; iSeason++) {
      std::string seasonString = getSeasonString(iSeason);
      for (int iDay = 0; iDay <7; iDay++) {
        std::string dayString = getDay(iDay+1);
        for (int iHour = 0; iHour <24; iHour++) {
          double g[10];
          double d = 0;
          int hour = iHour +1;
          if (iHour < 6) {
            hour = 1;
          }

          for (int i =0; i < 9; i++) {
            g[i] = dictionary.at(hour).at("Intercept").at(i) +
                   dictionary.at(hour).at(age).at(i) +
                   dictionary.at(hour).at(seasonString).at(i) +
                   dictionary.at(hour).at(computer).at(i) +
                   dictionary.at(hour).at(civstat).at(i) +
                   dictionary.at(hour).at(unemp).at(i) +
                   dictionary.at(hour).at(retired).at(i) +
                   dictionary.at(hour).at(edtry).at(i) +
                   dictionary.at(hour).at(famstat).at(i) +
                   dictionary.at(hour).at(dayString).at(i) +
                   dictionary.at(hour).at(sex).at(i);
            d += std::exp(g[i]);
          }
          g[9] = 0;
          d += 1;  // == std::exp(g[9]) == exp(0) == 1;
          double sum = 0;
          for (int i =0; i < 10; i++) {
            p[iSeason][iDay][iHour][i] = std::exp(g[i]) / d;
            sum += p[iSeason][iDay][iHour][i];
          }
          assert(std::abs(1.0-sum) <
            std::numeric_limits<double>::epsilon() * std::abs(1.0+sum)* 2);
        }
      }
    }
}

std::vector<double> Model_Activity::multinominal() {
    double p[4][7][24][10];
    multinominalP(p);

    std::vector<double> activities;

    static const int daysInMonth[] =
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int tsph = SimulationConfig::info.timeStepsPerHour;
    int hourCount = 0;
    int month = SimulationConfig::info.startMonth;
    int day = SimulationConfig::info.startDay -1;
    int dayOfWeek = SimulationConfig::info.startDayOfWeek-1;

    int season = getSeasonInt(month);

    for (int i = 0; i <= SimulationConfig::info.timeSteps; i++) {
        if (i % tsph == 0) {
            hourCount++;
            if (hourCount > 23) {
                hourCount = 0;
                day++;
                dayOfWeek++;
                if (dayOfWeek > 6) {
                  dayOfWeek = 0;
                }
                if (day > daysInMonth[month]) {
                  month++;
                  day = 1;
                  if (month == 13) {
                    month = 1;
                  }
                  season = getSeasonInt(month);
                }
            }
        }
        double act = multinominalActivity(p[season][dayOfWeek], hourCount);
        activities.push_back(act);
    }
    return activities;
}

void Model_Activity::parseConfiguration(const std::string filename) {
  namespace rx = rapidxml;
  rx::file<> xmlFile(filename.c_str());  // Default template is char
  rx::xml_document<> doc;
  doc.parse<0>(xmlFile.data());    // 0 means default parse flags
  rx::xml_node<> *root_node = doc.first_node("Activity");
  rx::xml_node<> *node = root_node->first_node();
  while (node) {
    std::string inter = node->name();
    if (inter.find("interval") != std::string::npos) {
      inter.erase(0, 8);
      int hour = std::stoi(inter);
      std::map<std::string, std::vector<double>> items;
      rx::xml_node<> *cnode = node->first_node();
      while (cnode) {
        std::string name = cnode->name();
        std::vector<std::string> tokProbs;
        Utility::splitCSV(cnode->value(), &tokProbs);
        std::vector<double> tokProbsD;
        for (std::string strProb : tokProbs) {
            tokProbsD.push_back(std::stod(strProb));
        }
        std::pair<std::string, std::vector<double>> x(name, tokProbsD);
        items.insert(x);
        cnode = cnode->next_sibling();
      }
      std::pair<int, std::map<std::string, std::vector<double>>>
        y(hour, items);
      dictionary.insert(y);
    } else {
      parseOther(node);
    }
    node = node->next_sibling();
  }
}

void Model_Activity::parseOther(rapidxml::xml_node<> *node) {
  node->name();
}
std::vector<double> Model_Activity::disaggregate() const {
    double probabilities[24][10];

    for (int hour = 0; hour < 24; hour++) {
        std::vector<std::string> tokProbs;
        Utility::splitCSV(probMap.at(hour), &tokProbs);
        int activity = 0;
        for (std::string strProb : tokProbs) {
            probabilities[hour][activity] = std::stod(strProb);
            activity++;
        }
    }

    std::vector<double> activities;
    int tsph = SimulationConfig::info.timeStepsPerHour;
    int hourCount = -1;
    for (int i = 0; i <= SimulationConfig::info.timeSteps; i++) {
        if (i % tsph == 0 || hourCount < 0) {
            hourCount++;
            if (hourCount == 24) {
                hourCount = 0;
            }
        }
        double drand = Utility::randomDouble(0.0, 1.0);
        double totalProb = 0;
        for (int j = 0; j < 10; j++) {
            totalProb += probabilities[hourCount][j];
            if (totalProb >= drand) {
                activities.push_back(j);
                break;
            }
        }
    }
    return activities;
}


void Model_Activity::setAge(const std::string & age) {
  this->age = age;
}

void Model_Activity::setComputer(const std::string & computer) {
  this->computer = computer;
}

void Model_Activity::setCivstat(const std::string & civstat) {
  this->civstat = civstat;
}

void Model_Activity::setUnemp(const std::string & unemp) {
  this->unemp = unemp;
}

void Model_Activity::setRetired(const std::string & retired) {
  this->retired = retired;
}

void Model_Activity::setEdtry(const std::string & edtry) {
  this->edtry = edtry;
}

void Model_Activity::setFamstat(const std::string & famstat) {
  this->famstat = famstat;
}

void Model_Activity::setSex(const std::string & sex) {
  this->sex = sex;
}

void Model_Activity::setProbMap(const std::map<int, std::string> & probMap) {
  this->probMap = probMap;
}
