// Copyright 2016 Jacob Chapman

#ifndef FMU_SOURCE_AGENT_H_
#define FMU_SOURCE_AGENT_H_

/**
 * @brief The Agent
 * @details Contains all information about the occupants and there associated interactions
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
    void setID(const int id);
    int getID() const;
 protected:
    int buildingID;
    int id;
 private:
};

#endif  // FMU_SOURCE_AGENT_H_
