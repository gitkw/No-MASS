// Copyright 2015 Jacob Chapman

#ifndef BUILDING_ZONE_H_
#define BUILDING_ZONE_H_
#include <string>
#include <vector>

#include "Configuration.hpp"
#include "Model_Windows.hpp"

 /**
  * @brief A zone within a building
  * @details A zone within a building
  */

class Building_Zone {
 public:
    Building_Zone();
    void step();
    void setup(const ConfigStructZone & zoneStruct);
    void setName(const std::string & name);
    void setCurrentOccupantGains(double currentOccupantGains);
    void setGroundFloor(bool groundFloor);
    void setActive(bool active);
    void setWindowDurationOpen(double windowDurationOpen);
    void setWindowState(bool windowState);
    void setLightState(bool lightState);
    void setBlindState(double state);
    void setHeatingState(double state);
    void setOccupantFraction(double occupantFraction);
    void setAppFraction(double appFraction);
    void setIDString(const std::string & idString);

    bool isActive() const;
    bool hasActivity(int activity) const;
    bool isNamed(const std::string & name) const;
    int getCurrentOccupantCount() const;
    int getId() const;
    int getNumberOfActivities() const;
    float getOccupantFraction() const;
    double getCurrentOccupantGains() const;
    double getWindowState() const;
    double getBlindState() const;
    double getHeatingState() const;
    double getGroundFloor() const;
    double getAirSystemSensibleHeatingRate() const;
    double getLightState() const;
    double getMeanAirTemperature() const;
    double getAirRelativeHumidity() const;
    double getMeanRadiantTemperature() const;
    double getDaylightingReferencePoint1Illuminance() const;
    double getWindowDurationOpen() const;

    std::vector<int> getActivities() const;

 private:
    bool groundFloor; //<! is the zone on the grounf floor
    bool active; //<! is the zone active, ie have occupants
    bool lightState; //<! the light state
    bool windowState; //<! the window state
    double currentOccupantGains; //<! the current mean gains for an occupant
    double blindState; //<! the shade state
    double occupantFraction; //<! the number of occupants (I dont think this is a fraction anymore)
    double heatingState; //<! the heating setpoint temperature
    double appFraction; //<! the average appliance gains
    double lightPower; //<! the power used by the lights in the zone

    std::vector<int> activities;   //<! the activites that take place in a zone
    std::string idString;  //<! the zones id as a string
    std::string name;  //<! the zones name
    int id;  //<! the zones id
    // datastore variables for faster lookup
    int variableNameBlindFraction;
    int variableNameNumberOfOccupants;
    int variableNameAverageGains;
    int variableNameLight;
    int variableNameHeating;
    int variableNameAppFraction;
    int variableNameZoneMeanAirTemperature;
    int variableNameZoneAirRelativeHumidity;
    int variableNameZoneMeanRadiantTemp;
    int variableNameDaylighting;
    int variableNameZoneAirHeating;
    std::vector<int> variableNameWindow;

};

#endif  // BUILDING_ZONE_H_
