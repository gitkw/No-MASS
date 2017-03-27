// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_BATTERY_H_
#define APPLIANCE_GROUP_BATTERY_H_

#include <vector>
#include "Appliance_Group.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */
template <class T>
class Appliance_Group_Battery : public Appliance_Group<T> {
 public:
    Appliance_Group_Battery(){}

    void setup(const std::vector<applianceStruct> & appBattery,
                                    const int & buildingID,
                                    const std::string & buildingString) {
      for (const applianceStruct s : appBattery) {
        this->appliances.push_back(T());
        this->appliances.back().setID(s.id);
        this->appliances.back().setEpsilon(s.epsilon);
        this->appliances.back().setAlpha(s.alpha);
        this->appliances.back().setGamma(s.gamma);
        this->appliances.back().setUpdate(s.update);
        this->appliances.back().setHoulyPriority(s.priority);
        this->appliances.back().setNeighbourhoodSimultion(s.neighbourhoodSimultion);
        this->appliances.back().setBuildingID(buildingID);
        this->appliances.back().setup();
        this->appliances.back().setIDString(buildingString + std::to_string(s.id));
        this->appliances.back().setupSave();
      }
      this->idString = buildingString + "_Battery_Sum";
      this->setupSave();
    }

    void step(Contract_Negotiation * app_negotiation) {
      this->reset();
      std::vector<int> pop = Utility::randomIntVect(this->appliances.size());
      for (int a : pop) {
        this->appliances[a].setPowerShortage(powerShortage);
        this->appliances[a].step();
        powerShortage -= this->appliances[a].getSupply();
        bool local = this->sendContractLocal(this->appliances[a], app_negotiation);
        this->appliances[a].setLocal(local);
        this->appliances[a].setGlobal(false);
        this->setParameters(this->appliances[a]);
      }
    }

void neighbourhoodNegotiationBattery(Contract_Negotiation * building_negotiation) {
  std::vector<int> pop = Utility::randomIntVect(this->appliances.size());
  for (int a : pop) {
    if(building_negotiation->getDifference() >= 0){
      break;
    }
    this->appliances[a].setPowerShortage(-building_negotiation->getDifference());
    this->appliances[a].stepNeighbourhood();
    bool local = false;
    if(this->appliances[a].getSupply() > 0 ){
      local = this->sendContractLocal(this->appliances[a], building_negotiation);
    }
    this->appliances[a].setGlobal(local);
  }
}

void neighbourhoodNegotiation(const Contract_Negotiation & building_negotiation) {
  this->reset();
  std::vector<int> pop = Utility::randomIntVect(this->appliances.size());
  for (int a : pop) {
    if (this->appliances[a].isGlobal()) {
      int appid = this->appliances[a].getID();
      int buildingID = this->appliances[a].getBuildingID();
      Contract c = building_negotiation.getContract(buildingID, appid);
      this->appliances[a].setGlobal(false);
      this->appliances[a].setReceived(c.received);
      this->appliances[a].setReceivedCost(c.receivedCost);
      this->appliances[a].setSupplyLeft(c.suppliedLeft);
    }

    this->setParameters(this->appliances[a]);
    this->appliances[a].saveNeighbourhood();
  }
}

bool sendContractGlobal(const Contract & c) {
  bool send = (c.suppliedLeft > 0);
  if (send) {
    Contract x = c;
    x.supplied = c.suppliedLeft;
    this->globalContracts.push_back(x);
  }
  return send;
}

    void setPowerShortage(double powerShortage){
      this->powerShortage = powerShortage;
    }

    double getPowerShortage() const {
      return powerShortage;
    }

    void globalCost(double globalCost) {
      for (T & l : this->appliances) {
        l.AddCost(globalCost);
      }
    }


 private:
   double powerShortage;

};

#endif  // APPLIANCE_GROUP_BATTERY_H_
