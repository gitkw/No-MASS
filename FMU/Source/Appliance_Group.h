// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_GROUP_H_
#define APPLIANCE_GROUP_H_

#include <vector>
#include "Appliance.h"
#include "DataStore.h"
#include "Utility.h"
#include "Contract.h"
#include "Contract_Negotiation.h"

/**
 * @brief Super class of each appliance type
 * @details An agent appliance that is used to house helper methods used in the sub classes
 */
template <class T>
class Appliance_Group {

 public:
    Appliance_Group(){};

    virtual void step(Contract_Negotiation * app_negotiation){
      std::vector<int> pop = Utility::randomIntVect(appliances.size());
      for (int a : pop) {
        appliances[a].step();
        bool local = sendContractLocal(appliances[a], app_negotiation);
        appliances[a].setLocal(local);
        appliances[a].setGlobal(false);
        parameters.power += appliances[a].getPower();
        parameters.supply += appliances[a].getSupply();

      }
    }

    bool sendContractLocal(const T & a, Contract_Negotiation * app_negotiation) {
      bool ret = (a.getPower() > 0 || a.getSupply() > 0);
      if (ret) {
        Contract c;
        c.id = a.getID();
        c.buildingID = a.getBuildingID();
        c.requested = a.getPower();
        c.priority = a.getPriority();
        c.supplied = a.getSupply();
        c.suppliedLeft = a.getSupply();
        c.suppliedCost = a.getSupplyCost();
        c.receivedCost = 0;
        c.received = 0;
        app_negotiation->submit(c);
      }
      return ret;
    }

    virtual void localNegotiation(const Contract_Negotiation & app_negotiation) {
      clearGlobalContracts();
      for (T & g : appliances) {
        if (g.isLocal()) {
          int appid = g.getID();
          int buildingID = g.getBuildingID();
          Contract c = app_negotiation.getContract(buildingID, appid);
          g.setGlobal(sendContractGlobal(c));
          g.setReceived(c.received);
          g.setReceivedCost(c.receivedCost);
          g.setSupplyLeft(c.suppliedLeft);
        }
        g.saveLocal();
      }
    }

    virtual void neighbourhoodNegotiation(const Contract_Negotiation & building_negotiation) {
      clearGlobalContracts();
      for (T & g : appliances) {
        if (g.isGlobal()) {
          int appid = g.getID();
          int buildingID = g.getBuildingID();
          Contract c = building_negotiation.getContract(buildingID, appid);
          g.setGlobal(sendContractGlobal(c));
          g.setReceived(c.received);
          g.setReceivedCost(c.receivedCost);
          g.setSupplyLeft(c.suppliedLeft);
        }
        g.saveNeighbourhood();
      }
    }

    virtual void globalNegotiation(const Contract_Negotiation & building_negotiation) {
      for (auto & g : appliances) {
        if (g.isGlobal()) {
          int appid = g.getID();
          int buildingID = g.getBuildingID();
          Contract c = building_negotiation.getContract(buildingID, appid);
          g.setReceived(c.received);
          g.setReceivedCost(c.receivedCost);
          g.setSupplyLeft(c.suppliedLeft);
        }

        parameters.supplyCost += g.getSupplyCost();
        parameters.received += g.getReceived();
        parameters.receivedCost += g.getReceivedCost();
        g.saveGlobal();
        g.save();
        g.clear();
      }
      if (!appliances.empty()) {
        DataStore::addValue(datastoreIDSupplied, parameters.supply);
        DataStore::addValue(datastoreIDSuppliedCost, parameters.supplyCost);
        DataStore::addValue(datastoreIDReceived, parameters.received);
        DataStore::addValue(datastoreIDRequested, parameters.power);
        DataStore::addValue(datastoreIDCost, parameters.receivedCost);
      }
    }


    virtual void reset() {
      parameters.power = 0.0;
      parameters.supply = 0.0;
      parameters.supplyCost = 0.0;
      parameters.received = 0.0;
      parameters.receivedCost = 0.0;
      clearGlobalContracts();
    }



    bool sendContractGlobal(const Contract & c) {
      bool send = (c.requested > c.received || c.suppliedLeft > 0);
      if (send) {
        Contract x = c;
        x.supplied = c.suppliedLeft;
        globalContracts.push_back(x);
      }
      return send;
    }

    void postprocess() {
      for (T & l : appliances) {
        l.postprocess();
      }
    }

    void addGlobalContactsTo(Contract_Negotiation * building_negotiation) const {
      for (const Contract & c : globalContracts) {
        building_negotiation->submit(c);
      }
    }

    void clearGlobalContracts(){
      globalContracts.clear();
    }

    double getSupply() const {
      return parameters.supply;
    }

    double getSupplyCost() const {
      return parameters.supplyCost;
    }

    double getPower() const {
      return parameters.power;
    }

    double getReceived() const {
      return parameters.received;
    }

    double getReceivedCost() const {
      return parameters.receivedCost;
    }

    T getApplianceAt(int BuildingID, int id){
      T app;
      for (T a : appliances) {
        if (a.getID() == id && a.getBuildingID() == BuildingID){
          app = a;
        }
      }
      return app;
    }

 protected:
   std::vector<T> appliances;
   std::vector<Contract> globalContracts;
   std::string idString;

   void setupSave() {
     if (!appliances.empty()) {
       datastoreIDSupplied = DataStore::addVariable(idString + "_supplied");
       datastoreIDSuppliedCost = DataStore::addVariable(idString + "_suppliedCost");
       datastoreIDReceived = DataStore::addVariable(idString + "_received");
       datastoreIDRequested = DataStore::addVariable(idString + "_requested");
       datastoreIDCost = DataStore::addVariable(idString + "_cost");
     }
   }


   ApplianceParameters parameters;
   int datastoreIDSupplied;
   int datastoreIDSuppliedCost;
   int datastoreIDReceived;
   int datastoreIDRequested;
   int datastoreIDCost;
};

#endif  // APPLIANCE_GROUP_H_
