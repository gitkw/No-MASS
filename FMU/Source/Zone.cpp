/*
 * File:   Zone.cpp
 * Author: jake
 *
 * Created on September 24, 2013, 10:11 AM
 */

#include <fstream>
#include <iostream>
#include "VariableStore.h"
#include "SimulationConfig.h"
#include "Zone.h"

Zone::Zone() {

}

Zone::Zone(std::string zoneName) : name(zoneName) {
    stepCount = -1;
    occupantFraction = 0;
    currentAgentGains = 0;
    blindState = 1;
    lightState = 0;
    windowState = 0;
    setActive(SimulationConfig::activeZone(&name));
    if(active){
        setGroundFloor(SimulationConfig::isZoneGroundFloor(&name));
        variableNameWindow = name + "WindowState";
        VariableStore::addVariable(variableNameWindow);
        variableNameBlindFraction = name + "BlindFraction";
        VariableStore::addVariable(variableNameBlindFraction);
        variableNameLight = name + "LightState";
        VariableStore::addVariable(variableNameLight);
        variableNameNumberOfOccupants = name + "NumberOfOccupants";
        VariableStore::addVariable(variableNameNumberOfOccupants);
        variableNameAverageGains = name + "AverageGains";
        VariableStore::addVariable(variableNameAverageGains);
    }
}

void Zone::setup(){
}

Zone::~Zone() {
}

void Zone::step() {
    stepCount++;
    if(active){
        VariableStore::addValue(variableNameNumberOfOccupants, occupantFraction);
        VariableStore::addValue(variableNameAverageGains, currentAgentGains);
        VariableStore::addValue(variableNameWindow, windowState);
        VariableStore::addValue(variableNameBlindFraction, blindState);
        VariableStore::addValue(variableNameLight, lightState);
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
    return VariableStore::getValue(name + "ZoneMeanAirTemperature");
}

double Zone::getAirRelativeHumidity() const {
    return VariableStore::getValue(name + "ZoneAirRelativeHumidity");
}

double Zone::getMeanRadiantTemperature() const {
    return VariableStore::getValue(name + "ZoneMeanRadiantTemperature");
}

double Zone::getDaylightingReferencePoint1Illuminance() const {
    return VariableStore::getValue(name + "DaylightingReferencePoint1Illuminance");
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

void Zone::setWindowState(bool windowState){
    this->windowState = windowState;
}
void Zone::setBlindState(double state){
    this->blindState = state;
}
