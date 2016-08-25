// Copyright 2016 Jacob Chapman

#ifndef LVN_NODE_H_
#define LVN_NODE_H_
#include <complex>
#include <string>
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
  void addNode(const LVN_Node & node);
  void setImpedance(const std::complex<double> & value);
  void setNominalVoltage(const std::complex<double> & value);
  void setNodeLoad(const std::complex<double> & nodeLoad);
  void forwardSweep();
  double checkTolerance();
  void backwardSweep(const std::complex<double> & parent_voltage);
  void runUntilConvergence(double tolerance);
  bool setPowerForID(const std::complex<double> & power, const int id);
  void setID(const int id);
  void setup();
  int getID() const;
  void addChildren(const std::vector<int> & ids);
  void save();

 private:
  void setComplexPower(const std::complex<double> & power);
  bool root;
  int id;
  std::complex<double> complexPower;
  std::complex<double> voltage;
  int iteration;
  //! Used for error calculations and decisive to start backward sweep.
  std::complex<double> slackVoltage;
  std::complex<double> currentLine;
  std::complex<double> currentLoad;
  std::complex<double> impedance;
  std::complex<double> nominalVoltage;
  std::complex<double> nodeLoad;

  std::string complexPowerStr;
  std::string voltageStr;
  std::string iterationStr;
  //! Used for error calculations and decisive to start backward sweep.
  std::string slackVoltageStr;
  std::string currentLineStr;
  std::string currentLoadStr;
  std::string impedanceStr;
  std::string nominalVoltageStr;
  std::string nodeLoadStr;

  std::vector<LVN_Node> joinedNodes;
};

#endif  // LVN_NODE_H_
