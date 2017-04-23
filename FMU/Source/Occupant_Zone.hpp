// Copyright 2016 Jacob Chapman

#ifndef OCCUPANT_ZONE_H_
#define OCCUPANT_ZONE_H_

#include <unordered_map>
#include <string>
#include <deque>
#include <memory>
#include <vector>

#include "Model_Activity.hpp"
#include "Model_Presence.hpp"
#include "StateMachine.hpp"
#include "State.hpp"
#include "Occupant_Action_Window_Stochastic.hpp"
#include "Occupant_Action_Window_Stochastic_BDI.hpp"
#include "Occupant_Action_Window_Learning.hpp"
#include "Occupant_Action_Lights.hpp"
#include "Occupant_Action_Lights_BDI.hpp"
#include "Occupant_Action_Shades.hpp"
#include "Occupant_Action_Shades_BDI.hpp"
#include "Occupant_Action_Appliance_BDI.hpp"
#include "Occupant_Action_Heat_Gains.hpp"
#include "Occupant_Action_HeatingSetPoints_Learning.hpp"

/**
* @brief The occupants understanding of a zone
* @details Contains all information about the occupants and there associated interactions with a zone
*/

class Occupant_Zone {
 public:
  Occupant_Zone();

  void postTimeStep();
  void postprocess();
  void setClo(double clo);
  void setMetabolicRate(double metabolicRate);
  void setup(int buildingID, const Building_Zone & buldingZone, int agentid,
            const ConfigStructAgent &agent);
  void step(const Building_Zone& zone, const Building_Zone& zonePrevious,
            const std::vector<double> &activities);
  void stepPre(const Building_Zone& zone, const Building_Zone& zonePrevious,
            const std::vector<double> &activities);

  void actionStep(int action, const Building_Zone &zone, bool inZone,
                  bool preZone, const std::vector<double> &activities);

  bool getDesiredWindowState() const;
  bool getDesiredLightState() const;
  bool isActionWindow() const;
  bool isActionLights() const;
  bool isActionShades() const;
  bool isActionHeatGains() const;
  bool isActionLearning() const;
  bool isActionAppliance() const;
  int getId() const;
  int getDesiredWindowDuration() const;
  double getDesiredShadeState() const;
  double getDesiredAppliance() const;
  double getDesiredHeatingSetPoint() const;
  double getPMV() const;
  double getHeatgains() const;

 private:
  void disableBDI();
  void enableBDI();
  void BDI(const std::vector<double> &activities);
  void setupWindows(int agentid, const ConfigStructAgent &agent,
                                            const Building_Zone & buldingZone);
  void setupLights(const ConfigStructAgent &agent, const Building_Zone & buldingZone);
  void setupShades(const ConfigStructAgent &agent, const Building_Zone & buldingZone);
  bool isInBuilding() const;
  void shuffleActions();

  int id;
  int buildingID;
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

#endif  // OCCUPANT_ZONE_H_
