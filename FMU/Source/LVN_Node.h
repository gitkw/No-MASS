// Copyright 2016 Jacob Chapman

#ifndef LVN_NODE_H_
#define LVN_NODE_H_

#include <vector>

/**
 * @brief Low voltage network for power flow analysis
 * @details Script copied from original Node class in Documents > LVNmodelling > powerFlowAnalysis.
 * Details on how to use the class for power flow analysis in there.
 * The class Node has been updated to take NodeLoad objects for definition of Complex Power.
 * NodeLoad objects inherit to HouseLoad, ApplianceLoad, ElectricVehicleLoad, PVLoad or any other, so that Node class
 * only needs to deal with NodeLoad objects. \n
 * Converted from Python by Jacob Chapman\n
 * Created by Ana Sancho on 12 Apr 2016.
 */

class LVN_Node {
 public:
  LVN_Node();
  void resetIterations();
  void addNode(LVN_Node node);
  void setImpedance(double value);
  void setNominalVoltage(double value);
  void setNodeLoad(double nodeLoad);
  void forwardSweep();
  double checkTolerance();
  void backwardSweep(double parent_voltage);
  void runUntilConvergence(double tolerance);
  bool setPowerForID(const double power, const int id);
  void setID(const int id);
  int getID() const;
  void addChildren(const std::vector<int> & ids);

 private:
  void setComplexPower(double power);
  bool root;
  double id;
  double complexPower;
  double voltage;
  double iteration;
  //! Used for error calculations and decisive to start backward sweep.
  double slackVoltage;
  double currentLine;
  double currentLoad;
  double impedance;
  double nominalVoltage;
  double nodeLoad;

  std::vector<LVN_Node> joinedNodes;
};

#endif  // LVN_NODE_H_
