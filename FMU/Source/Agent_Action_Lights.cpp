#include "Model_Lights.h"
#include "Agent_Action_Lights.h"

Agent_Action_Lights::Agent_Action_Lights()
{
    name = "Lights";
}


void Agent_Action_Lights::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){

    float Lumint = zone.getDaylightingReferencePoint1Illuminance();
    bool lightState = zone.getLightState();
    Model_Lights m_lightUsage;

    if (inZone && !previouslyInZone) {
            lightState = m_lightUsage.arrival(lightState, Lumint);
    } else if (inZone && previouslyInZone ) {
            lightState = m_lightUsage.intermediate(lightState, Lumint);
    } else if (!inZone && previouslyInZone ) {
            int pffs = getFutureDurationOfPresenceState(activities);
            lightState = m_lightUsage.departure(lightState, Lumint, pffs);
    }

    result = lightState;

}