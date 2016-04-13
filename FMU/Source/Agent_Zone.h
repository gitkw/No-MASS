/*
 * File:   Agent.h
 * Author: jake
 *
 * Created on September 15, 2013, 4:37 PM
 */

#ifndef AGENT_ZONE_H
#define	AGENT_ZONE_H
#include <unordered_map>
#include <string>
#include <memory>

#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"

#include "Agent_Action_Window.h"
#include "Agent_Action_Lights.h"
#include "Agent_Action_Learning.h"
#include "Agent_Action_Heating.h"
#include "Agent_Action_Shades.h"
#include "Agent_Action_Heat_Gains.h"
/**
 * @brief The Agent understand of a zone
 * @details Contains all information about the occupants and there associated interactions with a zone
 */

class Agent_Zone {
public:
    Agent_Zone();
    Agent_Zone(int id, int agentid, const agentStruct &agent);
    void step(const Building_Zone& zone, const Building_Zone& zonePrevious, const std::vector<double> &activities);

    void actionStep(int action,
      const Building_Zone &zone, bool inZone, bool preZone, const std::vector<double> &activities);
    void postprocess();
    void setClo(double clo);
    void setMetabolicRate(double metabolicRate);
    int getId() const;
    bool getDesiredWindowState() const;
    bool getDesiredLightState() const;
    double getDesiredShadeState() const;
    double getDesiredHeatingSetPoint() const;
    double getPMV() const;
    double getHeatgains() const;

private:

    bool isInBuilding() const;

    int id;
    int agentid;
    bool desiredLightState;
    bool desiredWindowState;
    double desiredShadeState;
    double desiredHeatingSetPoint;
    double heatgains;
    double clo;
    double metabolicRate;
    double pmv;
    double ppd;
    double previous_pmv;
    Agent_Action_Window aaw;
    Agent_Action_Lights aal;
    Agent_Action_Shades aas;
    Agent_Action_Heat_Gains aahg;
    Agent_Action_Learning aalearn;

    std::vector<int> availableActions;

};

#endif	/* AGENT_ZONE_H */
