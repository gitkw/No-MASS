// Copyright 2016 Jacob Chapman

#ifndef AGENT_H_
#define AGENT_H_

#include <string>
/**
 * @brief The Agent
 * @details Contains all information about the Agents
 */

class Agent {
 public:
    Agent();
    void setup();
    void step();
    void postprocess();
    void preprocess();
    void postTimeStep();

    void setBuildingID(const int id);
    void setIDString(const std::string & idString);
    void setID(const int id);
    int getID() const;
    int getBuildingID() const;
 protected:
    int buildingID;
    int id;
    std::string idString;
};

#endif  // AGENT_H_
