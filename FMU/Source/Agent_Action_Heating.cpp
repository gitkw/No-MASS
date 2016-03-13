// Copyright 2015 Jacob Chapman

#include <vector>

#include "Agent_Action_Heating.h"

Agent_Action_Heating::Agent_Action_Heating() {
    name = "Heating";
}

void Agent_Action_Heating::step(const Building_Zone& zone, bool inZone,
    bool previouslyInZone, const std::vector<double> &activities) {


    int stepCount = SimulationConfig::getStepCount();
    result = 1;
    if (activities.at(stepCount) == 9) {
        result = 0;
    }
}
