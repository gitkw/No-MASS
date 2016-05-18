// Copyright 2015 Jacob Chapman

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Building_Zone.h"

Building_Zone::Building_Zone() {}

Building_Zone::Building_Zone(const ZoneStruct & zoneStruct) :
                             id(zoneStruct.id),
                             name(zoneStruct.name),
                             activities(zoneStruct.activities) {
    occupantFraction = 0;
    currentAgentGains = 0;
    blindState = 1;
    lightState = 0;
    windowState = 0;

    setActive(SimulationConfig::activeZone(&name));
    if (active) {
        int windowCount = zoneStruct.windowCount;
        setGroundFloor(SimulationConfig::isZoneGroundFloor(&name));
        for (int i = 0; i < windowCount; i ++) {
          std::string windowName = name + "WindowState" + std::to_string(i);
          variableNameWindow.push_back(windowName);
          DataStore::addVariable(windowName);
        }

        variableNameBlindFraction = name + "BlindFraction";
        DataStore::addVariable(variableNameBlindFraction);
        variableNameLight = name + "LightState";
        DataStore::addVariable(variableNameLight);
        variableNameHeating = name + "Heating";
        DataStore::addVariable(variableNameHeating);
        variableNameNumberOfOccupants = name + "NumberOfOccupants";
        DataStore::addVariable(variableNameNumberOfOccupants);
        variableNameAverageGains = name + "AverageGains";
        DataStore::addVariable(variableNameAverageGains);
    }
}

void Building_Zone::setup() {}

void Building_Zone::step() {
    if (active) {
        DataStore::addValue(variableNameNumberOfOccupants, occupantFraction);
        DataStore::addValue(variableNameAverageGains, currentAgentGains);
        for (std::string & name : variableNameWindow) {
          DataStore::addValue(name, windowState);
        }
        DataStore::addValue(variableNameLight, lightState);
        // Shade in EP is
        // 1 is close
        // 0 is open
        // Shade in No-mass unshaded fraction
        // 1 is open
        // 0 is closed
        DataStore::addValue(variableNameBlindFraction, 1 - blindState);
        DataStore::addValue(variableNameHeating, heatingState);
    }
}

void Building_Zone::setName(std::string name) {
    this->name = name;
}

std::string Building_Zone::getName() const {
    return name;
}

int Building_Zone::getId() const {
    return id;
}

void Building_Zone::setCurrentAgentGains(double currentAgentGains) {
    this->currentAgentGains = currentAgentGains;
}

double Building_Zone::getCurrentAgentGains() const {
    return currentAgentGains;
}

double Building_Zone::getWindowState() const {
    return windowState;
}

double Building_Zone::getBlindState() const {
    return blindState;
}

double Building_Zone::getHeatingState() const {
    return heatingState;
}

std::vector<int> Building_Zone::getActivities() const {
  return activities;
}

void Building_Zone::setOccupantFraction(double occupantFraction) {
    this->occupantFraction = occupantFraction;
}

double Building_Zone::getMeanAirTemperature() const {
    return DataStore::getValue(name + "ZoneMeanAirTemperature");
}

double Building_Zone::getAirRelativeHumidity() const {
    return DataStore::getValue(name + "ZoneAirRelativeHumidity");
}

double Building_Zone::getMeanRadiantTemperature() const {
    return DataStore::getValue(name + "ZoneMeanRadiantTemperature");
}

double Building_Zone::getDaylightingReferencePoint1Illuminance() const {
    return DataStore::getValue(name + "DaylightingReferencePoint1Illuminance");
}

double Building_Zone::getAirSystemSensibleHeatingRate() const {
    return DataStore::getValue(name + "ZoneAirSystemSensibleHeatingRate");
}

float Building_Zone::getOccupantFraction() const {
    return occupantFraction;
}

void Building_Zone::setActive(bool active) {
    this->active = active;
}

bool Building_Zone::isActive() const {
    return active;
}

bool Building_Zone::hasActivity(int activity) const {
  return std::find(activities.begin(), activities.end(), activity)
      != activities.end();
}

void Building_Zone::setGroundFloor(bool groundFloor) {
    this->groundFloor = groundFloor;
}

double Building_Zone::getGroundFloor() const {
    return groundFloor;
}

void Building_Zone::setLightState(bool lightState) {
    this->lightState = lightState;
}

void Building_Zone::setHeatingState(double state) {
    this->heatingState = state;
}

double Building_Zone::getLightState() const {
    return lightState;
}

void Building_Zone::setWindowState(bool windowState) {
    this->windowState = windowState;
}

void Building_Zone::setBlindState(double state) {
    this->blindState = state;
}
