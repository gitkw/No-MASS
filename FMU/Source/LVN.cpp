// Copyright 2016 Jacob Chapman

#include "LVN_Node.h"
#include "SimulationConfig.h"
#include "LVN.h"

LVN::LVN() {}

void LVN::setup() {
  if (SimulationConfig::lvn.size() > 0) {
    enabled = true;
    for (const LVNNodeStruct & l : SimulationConfig::lvn) {
      if (l.parent == -1) {
        rootNode.setID(l.id);
        rootNode.addChildren(l.children);
          rootNode.setup();
        break;
      }
    }
  }
}

void LVN::postTimeStep() {
  if (enabled) {
    double tolerance = 0.00001;
    rootNode.setImpedance(0.0047);
    rootNode.runUntilConvergence(tolerance);
    rootNode.save();
    rootNode.resetIterations();
  }
}

void LVN::setPowerForID(const double power, const int id) {
  if (enabled) {
    double p = power;
    rootNode.setPowerForID(p, id);
  }
}
