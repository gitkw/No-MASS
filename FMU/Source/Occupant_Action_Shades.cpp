// Copyright 2015 Jacob Chapman

#include <iostream>
#include <vector>
#include "Utility.hpp"
#include "DataStore.hpp"
#include "Environment.hpp"
#include "Occupant_Action_Shades.hpp"
#include "Configuration.hpp"

Occupant_Action_Shades::Occupant_Action_Shades() {}

void Occupant_Action_Shades::setup(int shadeID) {
    ConfigStructShade ws = Configuration::shades.at(shadeID);
    m_blindUsage.setFullVars(ws.afullraise, ws.boutfullraise, ws.bsfullraise,
                  ws.bsfulllower, ws.boutfulllower, ws.afulllower);
    m_blindUsage.setDurationVars(ws.aSFlower, ws.bSFlower, ws.shapelower);
    m_blindUsage.setArrivalVars(ws.a01arr, ws.b01inarr, ws.b01sarr, ws.a10arr,
                  ws.b10inarr, ws.b10sarr);
    m_blindUsage.setInterVars(ws.a01int, ws.b01inint, ws.b01sint, ws.a10int,
                  ws.b10inint, ws.b10sint);
}

void Occupant_Action_Shades::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone) {
  double shadingFraction = zone.getBlindState();
  // we take the previous timestep shading state to compute Lumint
  // Evg: Outdoor illuminance in the horizontal plane without obstructions (lux)
  float Evg = Environment::getEVG();

  if (inZone && !previouslyInZone) {
    shadingFraction = m_blindUsage.arrival(shadingFraction, Lumint, Evg);
  } else {
    shadingFraction = m_blindUsage.departure(shadingFraction, Lumint, Evg);
  }
  result = shadingFraction;
}

void Occupant_Action_Shades::setIndoorIlluminance(const float lumint){
  //Lumint: Indoor illuminance next to window (lux)
   this->Lumint = lumint;
}
