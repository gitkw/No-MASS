#ifndef STATE_H
#define STATE_H
#include <string>
#include "Zone.h"



class State
{
    public:
        State();
        virtual ~State();

        virtual bool hasState(const int stateID) const;
        virtual State getState(const int stateID) const;

        void addState(State s);
        void setZonePtr(Zone* zoneptr);
        int getId() const;
        unsigned int numberOfSubStates() const;
        double getMetabolicRate() const;
        double getClo() const;
        std::string getActivity() const;
        std::string getLocation() const;
        Zone* getZonePtr() const;

    protected:
        int id;
        double metabolicRate;
        double clo;
        std::string activity;
        Zone* zone;

        std::vector<State> states;

    private:

};

#endif // STATE_H
