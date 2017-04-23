// Copyright 2015 Jacob Chapman

#ifndef QLEARNING_H_
#define QLEARNING_H_

#include <vector>
#include <string>

/**
* @brief The QLearning algorithm
* @details C++ implementation of the QLearning algorithm
*/

class QLearning {
 public:
    QLearning();
    int greedySelection(const int s) const;
    void updateQ(const int s, const int a, const double r, const int sp);
    void printQ() const;
    virtual void setId(const int id);
    void setFilename(const std::string filename);
    void setState(const int state);
    void setStates(const int states);
    void setActions(const int actions);
    void setReward(const double reward);
    void setEpsilon(const double epsilon);
    void setAlpha(double alpha);
    void setGamma(double gamma);
    void setUpdate(bool update);
    void setup();
    void setAction(const double action);
    double getAction();
    void learn();
    virtual void reset();

 protected:
    int id;
    int states = 0;
    int actions = 0;
    int state;
    int previous_state = 0;
    int action = 0;
    bool learnNext = false;
    double reward;
    double previous_reward;
    std::string filename;


 private:
    std::vector<std::vector<double>> qTable;

    double epsilon = 1;   // probability of a random action selection
    double alpha = 0.1;     // learning rate
    double gamma = 0.1;     // discount factor (how soon do you care)
    bool update;
    int getBestAction(const int s) const;

    int reward_name;
    int action_name;
    int state_name;
    int previous_state_name;
};

#endif  // QLEARNING_H_
