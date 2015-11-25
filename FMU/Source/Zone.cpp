// Copyright 2015 Jacob Chapman

#include <fstream>
#include <iostream>
#include <string>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Zone.h"

Zone::Zone() {
}

Zone::Zone(const std::string buldingName, const ZoneStruct zoneStruct)
  : name(zoneStruct.name) {
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
        variableNameNumberOfOccupants = name + "NumberOfOccupants";
        DataStore::addVariable(variableNameNumberOfOccupants);
        variableNameAverageGains = name + "AverageGains";
        DataStore::addVariable(variableNameAverageGains);
    }
}

void Zone::setup() {
}

void Zone::step() {
    if (active) {
        DataStore::addValue(variableNameNumberOfOccupants, occupantFraction);
        DataStore::addValue(variableNameAverageGains, currentAgentGains);
        for (std::string name : variableNameWindow) {
          DataStore::addValue(name, windowState);
        }
        DataStore::addValue(variableNameBlindFraction, blindState);
        DataStore::addValue(variableNameLight, lightState);
    }
}

void Zone::setName(std::string name) {
    this->name = name;
}

std::string Zone::getName() const {
    return name;
}


void Zone::setCurrentAgentGains(double currentAgentGains) {
    this->currentAgentGains = currentAgentGains;
}

double Zone::getCurrentAgentGains() const {
    return currentAgentGains;
}

double Zone::getWindowState() const {
    return windowState;
}

double Zone::getBlindState() const {
    return blindState;
}

void Zone::setOccupantFraction(double occupantFraction) {
    this->occupantFraction = occupantFraction;
}

double Zone::getMeanAirTemperature() const {
    return DataStore::getValue(name + "ZoneMeanAirTemperature");
}

double Zone::getAirRelativeHumidity() const {
    return DataStore::getValue(name + "ZoneAirRelativeHumidity");
}

double Zone::getMeanRadiantTemperature() const {
    return DataStore::getValue(name + "ZoneMeanRadiantTemperature");
}

double Zone::getDaylightingReferencePoint1Illuminance() const {
    return DataStore::getValue(name + "DaylightingReferencePoint1Illuminance");
}

double Zone::getAirSystemSensibleHeatingRate() const {
    return DataStore::getValue(name + "ZoneAirSystemSensibleHeatingRate");
}

float Zone::getOccupantFraction() const {
    return occupantFraction;
}

void Zone::setActive(bool active) {
    this->active = active;
}

bool Zone::isActive() const {
    return active;
}

void Zone::setGroundFloor(bool groundFloor) {
    this->groundFloor = groundFloor;
}

double Zone::getGroundFloor() const {
    return groundFloor;
}

void Zone::setLightState(bool lightState) {
    this->lightState = lightState;
}

double Zone::getLightState() const {
    return lightState;
}

void Zone::setWindowState(bool windowState) {
    this->windowState = windowState;
}

void Zone::setBlindState(double state) {
    this->blindState = state;
}
