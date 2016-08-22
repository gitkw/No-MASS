// Copyright 2016 Jacob Chapman

#include "Agent.h"

Agent::Agent() {}

void Agent::setup() {}

void Agent::preprocess() {}

void Agent::step() {}

void Agent::postprocess() {}

void Agent::postTimeStep() {}

void Agent::setBuildingID(const int id) {
    buildingID = id;
}

void Agent::setID(const int id) {
    this->id = id;
}

int Agent::getID() const {
    return id;
}
