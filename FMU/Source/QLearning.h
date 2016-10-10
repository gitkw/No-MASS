// Copyright 2015 Jacob Chapman

#ifndef QLEARNING_H_
#define QLEARNING_H_

#include <vector>
#include <string>

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
    virtual double learn();
    virtual void reset();

 protected:
    int id;
    int states = 0;
    int actions = 0;
    int state;
    int previous_state;
    int action;
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
};

#endif  // QLEARNING_H_
