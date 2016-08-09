// Copyright 2016 Jacob Chapman

/*
Convert from Python by Jacob
Created by Ana Sancho (user ezzas1)
on 12 Apr 2016 at 10:38.

Script copied from original Node class in Documents > LVNmodelling > powerFlowAnalysis.
Details on how to use the class for power flow analysis in there.
The class Node has been updated to take NodeLoad objects for definition of Complex Power.
NodeLoad objects inherit to HouseLoad, ApplianceLoad, ElectricVehicleLoad, PVLoad or any other, so that Node class
only needs to deal with NodeLoad objects.
*/


#ifndef LVN_NODE_H
#define LVN_NODE_H

#include <vector>
#include <complex>

class LVN_Node {

public:
  LVN_Node();
  void resetIterations();
  void addNode(LVN_Node node);
  void setImpedance(double value);
  void setNominalVoltage(double value);
  void setNodeLoad(double NodeLoad);
  void setComplexPower(int time_step);
  void forwardSweep();
  double checkTolerance();
  void backwardSweep(double parent_voltage);
  void runUntilConvergence(double tolerance);

private:

  double id;
  double complexPower;
  double voltage = 0.0;

  //double currentNode = None;
  double currentLine = 0.0;
  double currentLoad = 0.0;
  bool root = false;
  std::vector<LVN_Node> joinedNodes;
  double iteration = 0;
  double slackVoltage = 0.0; // slackVoltage used for error calculations and decisive to start backward sweep.
  double impedance = 0.0047; // ohm;
  double nominalVoltage = 220; // V;
  double nodeLoad;
};

#endif  /* LVN_NODE_H */
