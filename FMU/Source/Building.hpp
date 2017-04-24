// Copyright 2015 Jacob Chapman

#ifndef BUILDING_H_
#define BUILDING_H_
#include <vector>
#include <string>
#include "Configuration.hpp"
#include "Occupant.hpp"
#include "Building_Zone.hpp"
#include "Building_Appliances.hpp"

 /**
  * @brief A Building
  * @details A Building
  */

class Building {
 public:
  Building();
  void setup(const ConfigStructBuilding &buildingInput);
  void step();
  void postprocess();
  void preprocess();
  void postTimeStep();
  void stepAppliancesUse();
  void stepAppliancesUseBatteries(Contract_Negotiation * building_negotiation);
  void stepAppliancesNegotiationNeighbourhood(
                          const Contract_Negotiation & building_negotiation);
  void stepAppliancesNegotiation(
                          const Contract_Negotiation & building_negotiation);
  void addContactsTo(Contract_Negotiation * building_negotiation,
                    const bool battery);
  bool decisionBoolean(const double val1, const double val2) const;
  bool hasZone(const std::string& zoneName) const;
  int getID() const;
  double getPower() const;
  double decisionDoubleVec(const std::vector<double> & val,
                            const std::vector<double> & power,
                            const double currentState) const;

 private:
  void setOccupantGainsForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantWindowDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantLightDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantBlindDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantHeatDecisionsForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantCountForZone(std::shared_ptr<Building_Zone> zone);
  void setAppGainsForZone(std::shared_ptr<Building_Zone> zone);
  void buildingInteractions();

  int id; //<! id of the building
  std::string name; //<! name of the building
  std::vector<Occupant> population; //<! occupants in the building
  std::vector<std::shared_ptr<Building_Zone>> zones; //<! zones in the building
  Building_Appliances appliances; //<! building appliances
};

#endif  // BUILDING_H_
