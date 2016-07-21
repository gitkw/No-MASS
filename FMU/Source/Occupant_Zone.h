// Copyright 2016 Jacob Chapman


#ifndef Occupant_ZONE_H
#define	Occupant_ZONE_H
#include <unordered_map>
#include <string>
#include <deque>
#include <memory>

#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"
#include "Occupant_Action_Window_Stochastic.h"
#include "Occupant_Action_Window_Stochastic_BDI.h"
#include "Occupant_Action_Window_Learning.h"
#include "Occupant_Action_Lights.h"
#include "Occupant_Action_Lights_BDI.h"
#include "Occupant_Action_Shades.h"
#include "Occupant_Action_Shades_BDI.h"
#include "Occupant_Action_Appliance_BDI.h"
#include "Occupant_Action_Heat_Gains.h"
#include "Occupant_Action_HeatingSetPoints_Learning.h"
/**
 * @brief The Agent understand of a zone
 * @details Contains all information about the occupants and there associated interactions with a zone
 */

class Occupant_Zone {
public:
    Occupant_Zone();
    void setup(const Building_Zone & buldingZone, int agentid, const agentStruct &agent);
    void step(const Building_Zone& zone, const Building_Zone& zonePrevious,
              const std::vector<double> &activities);

    void actionStep(int action, const Building_Zone &zone, bool inZone,
                    bool preZone, const std::vector<double> &activities);
    void postTimeStep();
    void postprocess();

    void setClo(double clo);
    void setMetabolicRate(double metabolicRate);

    bool getDesiredWindowState() const;
    bool getDesiredLightState() const;
    int getId() const;
    int getDesiredWindowDuration() const;
    double getDesiredShadeState() const;
    double getDesiredAppliance() const;
    double getDesiredHeatingSetPoint() const;
    double getPMV() const;
    double getHeatgains() const;


    bool isActionWindow() const;
    bool isActionLights() const;
    bool isActionShades() const;
    bool isActionHeatGains() const;
    bool isActionLearning() const;
    bool isActionAppliance() const;

private:

    void setupWindows(int agentid, const agentStruct &agent, const Building_Zone & buldingZone);
    void setupLights(const agentStruct &agent, const Building_Zone & buldingZone);
    void setupShades(const agentStruct &agent, const Building_Zone & buldingZone);

    bool isInBuilding() const;
    void disableBDI();
    void enableBDI();
    void BDI(const std::vector<double> &activities);

    int id;
    bool ActionWindow;
    bool ActionLights;
    bool ActionShades;
    bool ActionHeatGains;
    bool ActionLearning;
    bool ActionAppliance;
    bool desiredLightState;
    bool desiredWindowState;
    bool hasBDI;
    double desiredShadeState;
    double desiredHeatingSetPoint;
    double desiredApplianceState;
    double heatgains;
    double clo;
    double metabolicRate;
    double pmv;
    double ppd;
    double previous_pmv;
    std::vector<int> availableActions;

    Occupant_Action_Window_Stochastic aaw;
    Occupant_Action_Window_Stochastic_BDI aawBDI;
    Occupant_Action_Window_Learning aawLearn;
    Occupant_Action_Lights aal;
    Occupant_Action_Lights_BDI aalBDI;
    Occupant_Action_Shades aas;
    Occupant_Action_Shades_BDI aasBDI;
    Occupant_Action_Heat_Gains aahg;
    Occupant_Action_HeatingSetPoints_Learning aalearn;
    Occupant_Action_Appliance_BDI aaa;

};

#endif	/* Occupant_ZONE_H */
