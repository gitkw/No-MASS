// Copyright 2015 Jacob Chapman

#include <vector>
#include "Model_Lights.h"
#include "Agent_Action_Lights.h"

Agent_Action_Lights::Agent_Action_Lights() {
    name = "Lights";
}


void Agent_Action_Lights::setOffDuringAudioVisual(bool OffDuringAudioVisual) {
  this->OffDuringAudioVisual = OffDuringAudioVisual;
}

void Agent_Action_Lights::setOffDuringSleep(bool OffDuringSleep) {
  this->OffDuringSleep = OffDuringSleep;
}

void Agent_Action_Lights::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone, const std::vector<double> &activities) {

    float Lumint = zone.getDaylightingReferencePoint1Illuminance();
    bool lightState = zone.getLightState();
    Model_Lights m_lightUsage;

    if (inZone && !previouslyInZone) {
            lightState = m_lightUsage.arrival(lightState, Lumint);
    } else if (inZone && previouslyInZone) {
            lightState = m_lightUsage.intermediate(lightState, Lumint);
    } else if (!inZone && previouslyInZone) {
            int pffs = getFutureDurationOfPresenceState(activities);
            lightState = m_lightUsage.departure(lightState, Lumint, pffs);
    }
    int stepCount = SimulationConfig::getStepCount();
    if (OffDuringSleep && activities.at(stepCount) == 0) {
        lightState = 0;
    }
    if (OffDuringAudioVisual && activities.at(stepCount) == 2) {
        lightState = 0;
    }
    result = lightState;
}
