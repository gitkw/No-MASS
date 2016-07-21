#ifndef Occupant_ACTION_WINDOW_H
#define Occupant_ACTION_WINDOW_H

#include "Occupant_Action.h"
#include "Model_Windows.h"
#include "Building_Zone.h"

class Occupant_Action_Window : public Occupant_Action {
    public:
        Occupant_Action_Window();
        void setup(int windowID, int id);
        void setDailyMeanTemperature(double dailyMeanTemperature);
        void saveResult();
        int durationOpen() const;

    protected:

        Model_Windows m_window;
        std::string variableNameWindowDesire;
        double dailyMeanTemperature;

    private:


};

#endif // Occupant_ACTION_WINDOW_H
