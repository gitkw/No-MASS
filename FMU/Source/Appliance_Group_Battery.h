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

    void setup(const std::vector<appBatteryStruct> & appBattery,
                                    const int & buildingID,
                                    const std::string & buildingString) {
      for (const appBatteryStruct s : appBattery) {
        this->appliances.push_back(T());
        this->appliances.back().setID(s.id);
        this->appliances.back().setEpsilon(s.epsilon);
        this->appliances.back().setAlpha(s.alpha);
        this->appliances.back().setGamma(s.gamma);
        this->appliances.back().setUpdate(s.update);
        this->appliances.back().setHoulyPriority(s.priority);
        this->appliances.back().setBuildingID(buildingID);
        this->appliances.back().setup();
        this->appliances.back().setIDString(buildingString + std::to_string(s.id));
        this->appliances.back().setupSave();
      }
      this->idString = buildingString + "_Battery_Sum";
      this->setupSave();
    }

    void step(Contract_Negotiation * app_negotiation){
      std::vector<int> pop = Utility::randomIntVect(this->appliances.size());
      for (int a : pop) {
        this->appliances[a].setPowerShortage(powerShortage);
        this->appliances[a].step();
        powerShortage -= this->appliances[a].getSupply();
        bool local = this->sendContractLocal(this->appliances[a], app_negotiation);
        this->appliances[a].setLocal(local);
        this->appliances[a].setGlobal(false);
        this->parameters.power += this->appliances[a].getPower();
        this->parameters.supply += this->appliances[a].getSupply();
      }
    }

    virtual void neighbourhoodNegotiation(const Contract_Negotiation & building_negotiation) {
      for (auto & g : this->appliances) {
        if (g.isGlobal()) {
          int appid = g.getID();
          int buildingID = g.getBuildingID();
          Contract c = building_negotiation.getContract(buildingID, appid);
          g.setGlobal(false);
          g.setReceived(c.received);
          g.setReceivedCost(c.receivedCost);
          g.setSupplyLeft(c.suppliedLeft);
        }
        g.saveNeighbourhood();
      }
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
