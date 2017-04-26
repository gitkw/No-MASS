# Implementation                       {#Implementation}

When launched, No-MASS first builds an agent population, assigning a profile to each member dependent on the input parameters supplied in a No-MASS configuration file.
These profiles influence the models predicting activities (e.g. sleeping, bathing, watching TV) and dependent behaviours (e.g. opening windows or lowering shading devices).
Once the agent profiles are assigned No-MASS proceeds according to one of two scenarios.
If the building is non-residential, chains of presence and absence are calculated; otherwise for residential buildings chains of activities are calculated, and the corresponding activity locations are assigned to each activity.
Communication with the building solver is then performed.
The building solver that we have coupled with No-MASS is EnergyPlus.
To achieve this we use the generic Functional Mockup Interface (FMI) co-simulation standard, so that we can also couple No-MASS with any other FMI compliant BPS software.
The building solver calculates the environmental conditions within the zones that the agents have been allocated to and parses the results to No-MASS via the FMI.
Each agent then retrieves its pre-processed location or presence (Page 2008).
State parameters are then set based on the activity (Jaboob 2015) that is performed, affecting the agents' clothing level, metabolic rate etc.
We then calculate the metabolic gains of each individual occupant.
Next we call models predicting the agents' use of shades (Haldi 2010), windows (Haldi 2009), lights (Reinhart 2004) and heating system setpoints.
To facilitate the modelling of agent social interactions we wrap these models in a social interaction framework; emulating negotiations through a vote casting mechanism.
The prediction of agents' heating setpoint choices is based on a reinforcement learning model, allowing agents to learn over a number of simulation replicates the setpoints that best maintain their PMV within acceptable bounds.
BDI rules are included to model relatively straight forward interactions for which data is scarce, such as closing shades for privacy in the home while showering.
Finally the results are parsed back to the building solver which calculates the environmental conditions arising from the modelled interactions at the next time step.


No-MASS was built from the ground up, C++ was chosen as the development language as it is simple to integrate with EnergyPlus (Crawley 2001), our chosen building simulation tool is also developed in C++.
Using the same language allows for easy communication between the two tools.
EnergyPlus developed by the US Department of Energy, is well tested, well documented and open source; allowing us to readily understand how to connect to it.
There are also two interfaces that allow other tools to interact with it, without altering the EnergyPlus source code.
The first is through the building controls virtual test bed (BCVTB) and the second is through the Functional Mockup Interface (FMI) (Nouidui 2014).
The No-MASS platform connects to EnergyPlus using FMI, which is an open standard so that No-MASS could in principle be integrated with any other FMI compliant simulation tool.
This is chosen over the BCVTB as it allows direct communication through C++ double precision arrays using predefined calling points, whereas BCVTB requires calls over sockets adding complexity and slowing the processing time.
The calling points are well documented, with No-MASS only using the initialise function, the receive an array of doubles function for the environmental variables and the send an array of doubles function for the occupant interactions.
The array of values that No-MASS receives at each time step is defined in the XML file ModelDescription.xml.
At the beginning of the time step the following environmental variables are received: horizontal sky illuminance, rain status, outdoor air dry-bulb temperature, zone air temperature, zone humidity, indoor radiant temperature and indoor illuminance.
Returned to EnergyPlus are the number of occupants in a zone, their metabolic gains, appliance gains, the window status, the blind shading fraction, the lighting status and the heating setpoint.
Due to the window, shading and location/presence models used within No-MASS a sub-hourly timestep is recommended (ie. 5 minutes), as longer timesteps may overestimate the implication of the occupant interactions.
For example the response time to an agent opening a window may be short with the room cooling in just a few minutes.
An agent can only respond at the next time step, if the timesteps are not sufficiently short in length, the open window may over cool the room.


@image latex FlowDiagramExistingModelsIncluded
@image html FlowDiagramExistingModelsIncluded.png
