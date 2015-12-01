// Copyright 2015 Jacob Chapman

#ifndef ZONE_H
#define	ZONE_H
#include <string>
#include <vector>

#include "SimulationConfig.h"
#include "Model_Windows.h"

class Zone {
public:
    Zone();
    Zone(std::string, const ZoneStruct zoneStruct);

    void step();
    void setup();
    void setName(std::string name);
    void setCurrentAgentGains(double currentAgentGains);
    void setGroundFloor(bool groundFloor);
    void setActive(bool active);
    void setWindowDurationOpen(double windowDurationOpen);
    void setWindowState(bool windowState);
    void setLightState(bool lightState);
    void setBlindState(double state);
    void setOccupantFraction(double occupantFraction);

    bool isActive() const;
    bool hasActivity(std::string activity) const;
    int getCurrentOccupantCount() const;
    float getOccupantFraction() const;
    double getCurrentAgentGains() const;
    double getWindowState() const;
    double getBlindState() const;
    double getGroundFloor() const;
    double getAirSystemSensibleHeatingRate() const;
    double getLightState() const;
    double getMeanAirTemperature() const;
    double getAirRelativeHumidity() const;
    double getMeanRadiantTemperature() const;
    double getDaylightingReferencePoint1Illuminance() const;
    double getWindowDurationOpen() const;
    std::string getName() const;

private:
  
    bool groundFloor;
    bool active;
    bool lightState;
    bool windowState;
    double currentAgentGains;
    double blindState;
    double occupantFraction;

    std::string name;
    std::string variableNameBlindFraction;
    std::string variableNameNumberOfOccupants;
    std::string variableNameAverageGains;
    std::string variableNameLight;
    std::vector<std::string> variableNameWindow;
    std::vector<std::string> activities;

};

#endif	/* ZONE_H */
