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

void Agent_Action_Lights::setOffShadeDuringOut(bool LightOffDuringOut) {
  this->OffDuringOut = OffDuringOut;
}

void Agent_Action_Lights::setOffDuringNight(bool OffDuringNight) {
  this->OffDuringNight = OffDuringNight;
}

void Agent_Action_Lights::step(const Zone& zone, bool inZone,
    bool previouslyInZone, const std::vector<double> &activities) {

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
    if (OffDuringNight && activities.at(stepCount) == 0) {
        lightState = 0;
    }
    result = OffDuringNight;
}
