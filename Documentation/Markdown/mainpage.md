# No-MASS                       {#mainpage}

[TOC]

## Introduction {#Introduction}
## Introduction

The No-MASS framework integrates existing models of occupant interaction and appliance usage into a tool that can be coupled with building or urban energy performance simulation tools, or run independantly.

## Pages {#Pages}
## Pages

* @ref compile
* @ref input
* @ref scripts
* @ref dependencies

## What is No-MASS? {#What}
## What is No-MASS?

A multi-agent stochastic simulation of occupants and appliances for use with building or urban scale simulations tools.
No-MASS using the generic FMI interface so that it can interface with any tool that meets the standard.

## What kind of data can I get from No-MASS? {#data}
## What kind of data can I get from No-MASS?

* Stochastic window openings predictions
* Stochastic external shade fractions
* Stochastic lighting predictions
* Stochastic occupant activity predictions
* Stochastic occupant presence predictions
* Stochastic large appliance use
* Stochastic small appliance use
* Machine learning applaince profile shifting
* Machine learning heating/ cooling setpoint predictions
* Beleif-Desire-Intent rules of occupant interactions

## Background {#Background}
## Background

The resources needed to sustain the world's ever expanding population are reaching new levels. It is therefore important to reduce global energy use arising from the contribution of fossil fuels and the associated emission of greenhouse gasses.
Building performance simulation is used to model the flows of energy in buildings and their systems at the design/ retrofit stage, to ensure they not only meet the demands of the occupants but also allow designers to test strategies for improving building performance.

Although a powerful building (re-)design decision support tool, building performance simulations can be subject to limitations. Studies have found that buildings may use twice as much energy as predicted at design stage.
Predicted building performance continues to deviate – sometimes considerably – from that which is observed post-build.
With the objective of addressing these limitations, and following the suggestions of Robinson (2011), our approach is to use multi-agent stochastic simulation for modelling occupant behaviour; to combine stochastic models into a single package that can be used to support building and urban performance simulation using a range of software.

A multi-agent simulation framework has been developed, which we have named Nottingham Multi-Agent Stochastic Simulation (No-MASS).
The No-MASS framework integrates existing stochastic models of occupant interaction into a tool that can be coupled with building or urban energy performance simulation tools.
The coupling allows for simulated occupants to make changes within the simulated building environment and receive responses arising from the effects of the interactions which may stimulate future interactions.
Current stochastic models do not cover all of the energy related behaviours of occupants, therefore a belief-desire-intent (BDI) rule system is used to model other interactions, supplementing the data-driven stochastic models.
For example switching off the light during sleep.
Agents can have unique desires causing changes within the environment that may be in conflict with the desires of other agents.
To solve this problem an agent social interaction model is developed to govern the interactions between agents.
For more complex interactions where BDI rules would be difficult to design, agent machine learning techniques are used, allowing the agent to learn how to respond to different stimuli.
