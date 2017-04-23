// Copyright 2015 Jacob Chapman

#include <algorithm>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Configuration.hpp"
#include "DataStore.hpp"
#include "Utility.hpp"
#include "QLearning.hpp"

QLearning::QLearning() {
  epsilon = Configuration::info.learnep;
  update = Configuration::info.learnupdate;
}

void QLearning::setup() {
    std::ifstream in_file;
    std::string filenameWithID = filename + std::to_string(id) + ".dat";
    in_file.open(filenameWithID, std::ios::binary);
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

int QLearning::greedySelection(const int s) const {
    int a;
    if (Utility::randomDouble(0, 1) < 1-epsilon) {
        a = getBestAction(s);
    } else {
        // selects a random action based on a uniform distribution
        a = Utility::randomDouble(0, actions);
    }
    return a;
}

int QLearning::getBestAction(const int s) const {
    std::vector<int> pop = Utility::randomIntVect(actions);
    int m = pop.back();
    pop.pop_back();
    for (const int i : pop) {
        if (qTable[s][m] < qTable[s][i]) {
            m = i;
        } else if (qTable[s][m] == qTable[s][i] && Utility::tossACoin()) {
            m = i;
        }
    }
    return m;
}

void QLearning::updateQ(const int s, const int a,
                        const double r, const int sp) {
  if (update) {
    double maxQ = *std::max_element(qTable[sp].begin(), qTable[sp].end());
    qTable[s][a] = qTable[s][a] + alpha * (r + gamma * maxQ - qTable[s][a]);
  }
}

void QLearning::printQ() const {
    std::ofstream myfile;
    myfile.open(filename + std::to_string(id) + ".dat");
    for (int i =0; i < states; i++) {
        myfile << qTable[i][0];
        for (int j =1; j < actions; j++) {
            myfile << "," << qTable[i][j];
        }
        myfile << std::endl;
    }
    myfile.close();
}

void QLearning::setId(const int id) {
    this->id = id;
    std::string idstr = std::to_string(id);
    reward_name = DataStore::addVariable(filename + "_reward" + idstr);
    action_name = DataStore::addVariable(filename + "_action" + idstr);
    state_name = DataStore::addVariable(filename + "_state" + idstr);
    previous_state_name = DataStore::addVariable(filename + "_previous_state" + idstr);
}

void QLearning::setState(const int state) {
    this->state = state;
}

void QLearning::setStates(const int states) {
    this->states = states;
}

void QLearning::setActions(const int actions) {
    this->actions = actions;
}

void QLearning::setReward(const double reward) {
    this->reward = reward;
}

void QLearning::learn() {
    DataStore::addValue(reward_name, reward);
    DataStore::addValue(action_name, action);
    DataStore::addValue(state_name, state);
    DataStore::addValue(previous_state_name, previous_state);
    updateQ(previous_state, action, reward, state);
}

double QLearning::getAction() {
  action = greedySelection(state);
  previous_state = state;
  return action;
}

void QLearning::reset() {}

void QLearning::setAction(const double action) {
  this->action = action;
}

void QLearning::setEpsilon(const double epsilon) {
  this->epsilon = epsilon;
}

void QLearning::setFilename(const std::string filename) {
  this->filename = filename;
}

void QLearning::setAlpha(double alpha) {
  this->alpha = alpha;
}

void QLearning::setGamma(double gamma) {
  this->gamma = gamma;
}

void QLearning::setUpdate(bool update) {
  this->update = update;
}
