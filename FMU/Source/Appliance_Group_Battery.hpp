// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_BATTERY_H_
#define APPLIANCE_GROUP_BATTERY_H_

#include <vector>
#include "Appliance_Group.hpp"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */
template <class T>
class Appliance_Group_Battery : public Appliance_Group<T> {
 public:
    Appliance_Group_Battery(){}

    void stepApp(T & a, Contract_Negotiation * app_negotiation) {
      a.setPowerShortage(powerShortage);
      a.step();
      powerShortage -= a.getSupply();
      bool local = this->sendContractLocal(a, app_negotiation);
      a.setLocal(local);
      a.setGlobal(false);
    }

    void neighbourhoodNegotiationBattery(Contract_Negotiation * building_negotiation) {
      this->shuffleAppliances();
      for (auto & a : this->appliances) {
        double diff = building_negotiation->getDifference();
        a.setPowerShortage(-diff);
        a.stepNeighbourhood();
        bool local = this->sendContractLocal(a, building_negotiation);
        a.setGlobal(local);
      }
    }

    void negotiationApp(const Contract_Negotiation & app_negotiation,
                                      T & app, const bool negotiate) {
        double received = app.getReceived();
        double receivedCost = app.getReceivedCost();
        double suppliedLeft = app.getSupplyLeft();
        if (negotiate) {
          int appid = app.getID();
          int buildingID = app.getBuildingID();
          Contract c = app_negotiation.getContract(buildingID, appid);
          //app.setGlobal(sendContractGlobal(c));
          received = c.received;
          receivedCost = c.receivedCost;
          suppliedLeft = c.suppliedLeft;
        }
        app.setReceived(received);
        app.setReceivedCost(receivedCost);
        app.setSupplyLeft(suppliedLeft);
        app.setGlobal(false);
    }

    void negotiationAppGlobal(const Contract_Negotiation & app_negotiation,
                                      T & app, const bool negotiate) {
        // app.setPower(0.0);
        // app.setSupply(0.0);
        // app.setSupplyCost(0.0);
        // app.setReceivedCost(0.0);
        // app.setSupplyLeft(0.0);
        negotiationApp(app_negotiation, app, negotiate);
    }

    virtual bool sendCondition(const Contract & c){
      return (c.suppliedLeft > 0);
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
