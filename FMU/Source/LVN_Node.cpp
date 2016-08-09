// Copyright 2016 Jacob Chapman
#include <complex>
#include "LVN_Node.h"
/*
import numpy as np
import pandas as pd
import cmath
from PowerFunctions import phasor, phasor_arr
*/
LVN_Node::LVN_Node() {
}


void LVN_Node::resetIterations() {
    /*
    For each time step that we want to achieve convergence,
    we need to reset the iteration counter. The iteration
    counter controls when the terminal nodes are assumed
    to have nominal voltage.
    */
    iteration = 0;
}

void LVN_Node::addNode(LVN_Node node) {
        /*
        Add node to object node, to create a network.
        :param node: node to be added.
        :return: Appends the added node to the .joinedNodes list.
        */
        joinedNodes.push_back(node);
}

void LVN_Node::setImpedance(double value) {
        /*
        Set value of the cable impedance between self node and the parent node.
        :param value: value of impedance in Ohms
        :return:
        */
        impedance = value;
}
void LVN_Node::setNominalVoltage(double value) {
        /*
        Set value of the nominal voltage.
        :param value: value of nominal voltage in Volts.
        :return:
        */
        nominalVoltage = value;
}
void LVN_Node::setNodeLoad(double NodeLoad) {
        /*
        Assign the corresponding NodeLoad to the node, such as HouseLoad, ApplianceLoad, etc.
        :param NodeLoad: NodeLoad object. NodeLoad object contains NodeLoad.profile, an array of the power profile.
        */
        nodeLoad = NodeLoad;
}
void LVN_Node::setComplexPower(int time_step) {
        /*
        Set complex power drawn from the node, using NodeLoad.getComplexPowerValue(time_ix).
        :param time_ix: Index of the array corresponding to time step.
        :return: - adds complex power value to node.
        */
        // Load from appliances
        //  complexPower = nodeLoad.getComplexPowerValue(time_step);
        complexPower = 0 + time_step;
}
void LVN_Node::forwardSweep() {
        /*
        On the forward sweep currents of the network are updated as:
        V[j]   = V[j+1] + Iline[j+1]*Z[j+1]         //with only taking one branch (j+1) from all the k branches attached to j-1.
        Iload[j] = (S[j]/V[j])*                     // is complex conjugate
        Iline[j] = Iload[j] + sum_k(Iline[k]*Z[k]), for k being all child branches coming from node j.
                                                    (All currents arriving to j)

        If node is a terminal node, then voltage must be assumed (on the first iteration) to be the nominal voltage.
        :return: -nothing - updates voltages/currents
        */

        for (LVN_Node node : joinedNodes) {
            //  # print node.id  // uncomment if want to check sweep order.
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
        for (LVN_Node childnode : joinedNodes) {  // calculate line current
            currentLine = currentLine + childnode.currentLine;
            //  #first calculate   Iline[j] = sum_k(Iline[k]*Z[k])
        }
        // second calculate  Iline[j] = Iload[j] + Iline[j]
        currentLine = currentLoad + currentLine;
        iteration = iteration +1;
}

double LVN_Node::checkTolerance() {
        /*
        Evaluates error between voltage of rootNode (closest one to source) and nominal voltage.
        :return: error
        What we need to calculate is
            error = nominalVoltage - Vslack (see figure attached in report)
            error = abs(nominalVoltage - voltage) // this is wrong, since the voltage drop between Vroot and Vslack is not being considered.
        */
        slackVoltage = voltage + currentLine*impedance;
        double error = std::abs(nominalVoltage - slackVoltage);
        return error;
}
void LVN_Node::backwardSweep(double parent_voltage) {
        /*
        On the backward sweep, voltages of the network are updated as:
        V[j] = V[j-1] - Iline[j]*z[j] // unless you are root, and then: V[j-1] = Vnominal
        :param parent_voltage:  As input needs to be the voltage of the root node (set to nominal voltage before starting
                                backward sweep.
                                After, recursively corresponds to V[j-1].
        :return: - Only updates voltages of all nodes.
        */
        // # V[j] = V[j-1] - Iline[j]*z[j]
        voltage = parent_voltage - currentLine * impedance;
        //  # do this for all nodes: each childnode is looped,
        for (LVN_Node childnode : joinedNodes) {
            //  # considering all childchildnodes it may have.
            //  # print childnode.id
            childnode.backwardSweep(voltage);
        }
}

void LVN_Node::runUntilConvergence(double tolerance) {
        /*
        runUntilConvergence should only be run on the rootNode.
        Runs a forward sweep. Calculates error. If error is smaller than tolerance, convergence is achieved. Otherwise
        starts backward sweep. And again.
        :param tolerance: tolerance to be admissible for the error.
        :return: Nothing. but voltages and currents of convergence will be saved in node.voltage, node.currentLoad.
        */
        while (true) {
            forwardSweep();  //  # do forward sweep
            double error = checkTolerance();  //  # check error
            //  # print error
            //  # convergence accepted: break loop and finish run
            if (error < tolerance) {
                // stop if converges
                // return showResultsTable()
                break;
            } else {
                //  Otherwise do backward sweep,
                //  after setting voltage of rootNode to nominal.
                slackVoltage = nominalVoltage;
                //  voltage = nominalVoltage
                backwardSweep(slackVoltage);
                //  print 'ERROR: ',error, ' - ' ,tolerance, '\n'
          }
        }
}
