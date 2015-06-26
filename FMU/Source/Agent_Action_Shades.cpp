
#include "VariableStore.h"
#include "Model_ExternalShading.h"
#include "Agent_Action_Shades.h"

Agent_Action_Shades::Agent_Action_Shades()
{
    //ctor
}

void Agent_Action_Shades::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
        double shadingFraction = zone.getBlindState();
        // we take the previous timestep shading state to compute Lumint
        //std::cout << "name: " << name << " Lumint: " << Lumint << std::endl;
        // Evg: Outdoor illuminance in the horizontal plane without obstructions (lux)
        float Evg = VariableStore::getValue("EnvironmentSiteExteriorHorizontalSkyIlluminance");
        // Lumint: Indoor illuminance next to window (lux)
        float Lumint = zone.getDaylightingReferencePoint1Illuminance();
        Model_ExternalShading m_blindUsage;
        if(inZone && !previouslyInZone ) {
                shadingFraction = m_blindUsage.arrival(shadingFraction,Lumint,Evg);
        } else {
                shadingFraction = m_blindUsage.departure(shadingFraction,Lumint,Evg);
        }
        result = shadingFraction;
}
