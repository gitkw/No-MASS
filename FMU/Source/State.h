#ifndef STATE_H
#define STATE_H
#include <string>
#include <memory>
#include "Building_Zone.h"

class State
{
    public:
        State();
        virtual ~State();
        virtual bool hasState(const int stateID) const;
        virtual State getState(const int stateID) const;

        void addState(State s);
        void setZonePtr(std::shared_ptr<Building_Zone> zoneptr);
        int getId() const;
        unsigned int numberOfSubStates() const;
        double getMetabolicRate() const;
        double getClo() const;
        std::string getActivity() const;
        std::string getLocation() const;
        std::shared_ptr<Building_Zone> getZonePtr() const;

    protected:
        int id;
        double metabolicRate;
        double clo;
        std::string activity;
        std::vector<State> states;
        std::shared_ptr<Building_Zone> zone;
};

#endif // STATE_H
