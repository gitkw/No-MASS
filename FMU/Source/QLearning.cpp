// Copyright 2015 Jacob Chapman

#include <algorithm>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "SimulationConfig.h"
#include "Utility.h"
#include "QLearning.h"

QLearning::QLearning() {}

void QLearning::setup() {
    epsilon = SimulationConfig::info.qlearnep;
    std::ifstream in_file;
    in_file.open("rlearning" + std::to_string(id) + ".dat");
    if (in_file.fail()) {
        for (int i =0; i < states; i++) {
            qTable.push_back(std::vector<double>());
            for (int j =0; j < actions; j++) {
                qTable[i].push_back(0);
            }
        }
    } else {
        for (int count =0; count < states; count++) {
            qTable.push_back(std::vector<double>());
            std::string a;
            in_file >> a;
            std::stringstream ss(a);
            std::string item;
            while (std::getline(ss, item, ',')) {
                qTable[count].push_back(std::stod(item));
            }
        }
    }
}

int QLearning::greedySelection(int s) {
    int a;
    if (Utility::randomDouble(0, 1) < 1-epsilon) {
        a = getBestAction(s);
    } else {
        // selects a random action based on a uniform distribution
        a = Utility::randomDouble(0, actions);
    }
    return a;
}

int QLearning::getBestAction(int s) {
    int m = 0;
    for (int i =1; i < actions; i++) {
        if (qTable[s][m] < qTable[s][i]) {
            m = i;
        } else if (qTable[s][m] == qTable[s][i] && Utility::tossACoin()) {
            m = i;
        }
    }
    return m;
}

void QLearning::updateQ(int s, int a, double r, int sp) {
  double maxQ = *std::max_element(qTable[sp].begin(), qTable[sp].end());
  qTable[s][a] = qTable[s][a] + alpha * (r + gamma * maxQ - qTable[s][a]);
}

void QLearning::printQ() {
    std::ofstream myfile;
    myfile.open("rlearning" + std::to_string(id) + ".dat");
    for (int i =0; i < states; i++) {
        for (int j =0; j < actions; j++) {
            myfile  << qTable[i][j]  << ",";
        }
        myfile  <<  std::endl;
    }
    myfile.close();
}

void QLearning::setId(int id) {
    this->id = id;
}

void QLearning::setState(int state) {
    this->state = state;
}

void QLearning::setReward(double reward) {
    this->reward = reward;
}

double QLearning::learn(const Zone &zone) {
  return 0;
}

void QLearning::setEpsilon(double epsilon) {
  this->epsilon = epsilon;
}
