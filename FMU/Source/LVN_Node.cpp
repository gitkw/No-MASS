// Copyright 2016 Jacob Chapman
#include <complex>
#include <vector>
#include <string>
#include "Configuration.hpp"
#include "DataStore.hpp"
#include "LVN_Node.hpp"

LVN_Node::LVN_Node() {
  root = false;
  voltage = 0.0;
  iteration = 0;
  slackVoltage = 0.0;
  currentLine = 0.0;
  currentLoad = 0.0;
  impedance = 0.0047;
  nominalVoltage = 220;
}

void LVN_Node::setup() {
  std::string idStr = std::to_string(id);
  complexPowerStr = DataStore::addVariable("LVN_complexPower_" + idStr);
  voltageStr = DataStore::addVariable("LVN_voltage_" + idStr);
  iterationStr = DataStore::addVariable("LVN_iteration_" + idStr);
  slackVoltageStr = DataStore::addVariable("LVN_slackVoltage_" + idStr);
  currentLineStr = DataStore::addVariable("LVN_currentLine_" + idStr);
  currentLoadStr = DataStore::addVariable("LVN_currentLoad_" + idStr);
  impedanceStr = DataStore::addVariable("LVN_impedance_" + idStr);
  nominalVoltageStr = DataStore::addVariable("LVN_nominalVoltage_" + idStr);
  nodeLoadStr = DataStore::addVariable("LVN_nodeLoad_" + idStr);
}

/**
 * @brief For each time step that we want to achieve convergence,
 * we need to reset the iteration counter. The iteration
 * counter controls when the terminal nodes are assumed
 * to have nominal voltage.
 */
void LVN_Node::resetIterations() {
    iteration = 0;
}

/**
 * @brief Add node to object node, to create a network.
 * @param node: node to be added.
 */
void LVN_Node::addNode(const LVN_Node & node) {
  joinedNodes.push_back(node);
}

/**
 * @brief Set value of the cable impedance between self node and the parent node.
 * @param value: value of impedance in Ohms
 */
void LVN_Node::setImpedance(const std::complex<double> & value) {
  this->impedance = value;
}

/**
 * @brief Set value of the nominal voltage.
 * @param value: value of nominal voltage in Volts.
 */
void LVN_Node::setNominalVoltage(const std::complex<double> & value) {
  this->nominalVoltage = value;
}

/**
 * @brief Assign the corresponding NodeLoad to the node, such as HouseLoad, ApplianceLoad, etc.
 * @param NodeLoad: NodeLoad object contains NodeLoad.profile, an array of the power profile.
 */
void LVN_Node::setNodeLoad(const std::complex<double> & nodeLoad) {
  this->nodeLoad = nodeLoad;
}

/**
 * @brief Set complex power drawn from the node, using NodeLoad.getComplexPowerValue(time_ix).
 * @param time_ix Index of the array corresponding to time step.
 */
void LVN_Node::setComplexPower(const std::complex<double> & power) {
  // Load from appliances
  complexPower = power;
}

/**
 * @brief On the forward sweep currents of the network are updated
 * @details V[j]   = V[j+1] + Iline[j+1]*Z[j+1] \n
 * with only taking one branch (j+1) from all the k branches attached to j-1.\n
 * Iload[j] = (S[j]/V[j])*                     // is complex conjugate\n
 * Iline[j] = Iload[j] + sum_k(Iline[k]*Z[k]), for k being all child branches coming from node j.\n
 *                                             (All currents arriving to j)\n
 * If node is a terminal node, then voltage must be assumed (on the first iteration) to be the nominal voltage.\n
 */
void LVN_Node::forwardSweep() {
  for (LVN_Node & node : joinedNodes) {
    node.forwardSweep();
  }
  //  for first iteration,
  //  if node is terminal node, voltage is set to nominalVoltage.
  if (joinedNodes.empty() && iteration == 0) {
    voltage = nominalVoltage;
  } else if (!joinedNodes.empty()) {
    // if it has child nodes calculate voltage
    // with V[j] = V[j+1] + Iline[j+1]*Z[j+1]
    voltage = joinedNodes[0].voltage +
                joinedNodes[0].currentLine * joinedNodes[0].impedance;
    // we use joinedNodes[0] since we only need one
    // of the possible branches it may have.
  }
  // calculate load current: Iload[j] = (S[j]/V[j])*
  currentLoad = complexPower / voltage;
  // set to zero in order to sum up all the child nodes
  currentLine = 0.0;
  for (LVN_Node & childnode : joinedNodes) {  // calculate line current
    currentLine = currentLine + childnode.currentLine;
    //  #first calculate   Iline[j] = sum_k(Iline[k]*Z[k])
  }
  // second calculate  Iline[j] = Iload[j] + Iline[j]
  currentLine = currentLoad + currentLine;
  iteration = iteration +1;
}

/**
 * @brief Evaluates error between voltage of rootNode (closest one to source) and nominal voltage.
 * @return error
 * @details What we need to calculate is:\n
 * error = nominalVoltage - Vslack (see figure attached in report)\n
 * error = abs(nominalVoltage - voltage) // this is wrong,\n
 * since the voltage drop between Vroot and Vslack is not being considered.\n
 */
double LVN_Node::checkTolerance() {
  slackVoltage = voltage + currentLine * impedance;
  double error = std::abs(nominalVoltage - slackVoltage);
  return error;
}

/**
 * @brief On the backward sweep, voltages of the network are updated.
 * @param parent_voltage:  As input needs to be the voltage of the root node
 * (set to nominal voltage before starting backward sweep.
 * After, recursively corresponds to V[j-1].
 * @details V[j] = V[j-1] - Iline[j]*z[j]
 * unless you are root, and then: V[j-1] = Vnominal
 */
void LVN_Node::backwardSweep(const std::complex<double> & parent_voltage) {
  // # V[j] = V[j-1] - Iline[j]*z[j]
  voltage = parent_voltage - currentLine * impedance;
  //  # do this for all nodes: each childnode is looped,
  for (LVN_Node & childnode : joinedNodes) {
    //  # considering all childchildnodes it may have.
    childnode.backwardSweep(voltage);
  }
}

/**
 * @brief runUntilConvergence should only be run on the rootNode.
 * @param tolerance tolerance to be admissible for the error.
 * @details Runs a forward sweep. Calculates error.\n
 * If error is smaller than tolerance, convergence is achieved.\n
 * Otherwise starts backward sweep. And again.\n
 * voltages and currents of convergence are saved in voltage and currentLoad.\n
 */
void LVN_Node::runUntilConvergence(double tolerance) {
  forwardSweep();  // do forward sweep
  double error = checkTolerance();  // check error
  // convergence accepted, finish run
  if (error >= tolerance) {
    //  No convergence do backward sweep,
    //  after setting voltage of rootNode to nominal.
    slackVoltage = nominalVoltage;
    backwardSweep(slackVoltage);
    runUntilConvergence(tolerance);
  }
}

void LVN_Node::save() {
  for (LVN_Node & childnode : joinedNodes) {
    //  # considering all childchildnodes it may have.
    childnode.save();
  }
  DataStore::addValue(complexPowerStr, complexPower.real());
  DataStore::addValue(voltageStr, voltage.real());
  DataStore::addValue(iterationStr, iteration);
  DataStore::addValue(slackVoltageStr, slackVoltage.real());
  DataStore::addValue(currentLineStr, currentLine.real());
  DataStore::addValue(currentLoadStr, currentLoad.real());
  DataStore::addValue(impedanceStr, impedance.real());
  DataStore::addValue(nominalVoltageStr, nominalVoltage.real());
  DataStore::addValue(nodeLoadStr, nodeLoad.real());
}

/**
 * @brief set the power for node at the ID.
 * @param power the nodes power.
 * @param id of the node we are searching for.
 * @return true if node is found.
 */
bool LVN_Node::setPowerForID(const std::complex<double> & power, const int id) {
  bool found = this->id == id;
  if (found) {
    setComplexPower(power);
  } else {
    for (LVN_Node & childnode : joinedNodes) {
      found = childnode.setPowerForID(power, id);
      if (found) {
        break;
      }
    }
  }
  return found;
}

void LVN_Node::addChildren(const std::vector<int> & ids) {
  for (const int & i : ids) {
    for (const ConfigStructLVNNode & l : Configuration::lvn) {
      if (i == l.id) {
        joinedNodes.push_back(LVN_Node());
        joinedNodes.back().setID(l.id);
        joinedNodes.back().setImpedance(l.impedance);
        joinedNodes.back().addChildren(l.children);
        joinedNodes.back().setup();
        break;
      }
    }
  }
}

void LVN_Node::setID(const int id) {
    this->id = id;
}

int LVN_Node::getID() const {
    return id;
}
