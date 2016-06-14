// Copyright 2015 Jacob Chapman

#include <iostream>
#include <vector>
#include "Utility.h"
#include "DataStore.h"
#include "Agent_Action_Shades.h"
#include "SimulationConfig.h"

Agent_Action_Shades::Agent_Action_Shades() {
  ShadeClosedDuringSleep = 0.0;
  ShadeClosedDuringWashing = 0.0;
  ShadeClosedDuringNight = 0.0;
  ShadeClosedDuringAudioVisual = 0.0;
}

void Agent_Action_Shades::setClosedDuringWashing(
  double ShadeClosedDuringWashing) {
  this->ShadeClosedDuringWashing = ShadeClosedDuringWashing;
}

void Agent_Action_Shades::setClosedDuringSleep(double ShadeClosedDuringSleep) {
  this->ShadeClosedDuringSleep = ShadeClosedDuringSleep;
}

void Agent_Action_Shades::setClosedDuringNight(double ShadeClosedDuringNight) {
  this->ShadeClosedDuringNight = ShadeClosedDuringNight;
}
void Agent_Action_Shades::setClosedDuringAudioVisual(
  double ShadeClosedDuringAudioVisual) {
  this->ShadeClosedDuringAudioVisual = ShadeClosedDuringAudioVisual;
}

void Agent_Action_Shades::setup(int shadeID) {
    shadeStruct ws = SimulationConfig::shades.at(shadeID);
    m_blindUsage.setFullVars(ws.afullraise, ws.boutfullraise, ws.bsfullraise,
                  ws.bsfulllower, ws.boutfulllower, ws.afulllower);
    m_blindUsage.setDurationVars(ws.aSFlower, ws.bSFlower, ws.shapelower);
    m_blindUsage.setArrivalVars(ws.a01arr, ws.b01inarr, ws.b01sarr, ws.a10arr,
                  ws.b10inarr, ws.b10sarr);
    m_blindUsage.setInterVars(ws.a01int, ws.b01inint, ws.b01sint, ws.a10int,
                  ws.b10inint, ws.b10sint);
}

void Agent_Action_Shades::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone) {
  double shadingFraction = zone.getBlindState();
  // we take the previous timestep shading state to compute Lumint
  // Evg: Outdoor illuminance in the horizontal plane without obstructions (lux)
  float Evg =
    DataStore::getValue("EnvironmentSiteExteriorHorizontalSkyIlluminance");
  // Lumint: Indoor illuminance next to window (lux)
  float Lumint = zone.getDaylightingReferencePoint1Illuminance();

  if (inZone && !previouslyInZone) {
    shadingFraction = m_blindUsage.arrival(shadingFraction, Lumint, Evg);
  } else {
    shadingFraction = m_blindUsage.departure(shadingFraction, Lumint, Evg);
  }
  result = shadingFraction;
}

bool Agent_Action_Shades::BDI(const std::vector<double> &activities) {
  bool bdi = false;
  int stepCount = SimulationConfig::getStepCount();
  if (ShadeClosedDuringSleep > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 0) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringWashing > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 6) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringAudioVisual > Utility::randomDouble(0, 1) &&
      activities.at(stepCount) == 2) {
      result = 0;
      bdi = true;
  }
  if (ShadeClosedDuringNight > Utility::randomDouble(0, 1)) {
      int hour = DataStore::getValue("hourOfDay");
      if (hour < 8) {
        result = 0;
        bdi = true;
      } else if (hour > 20) {
        result = 0;
        bdi = true;
      }
  }
  return bdi;
}
