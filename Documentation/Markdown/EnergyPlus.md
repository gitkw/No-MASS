# IDF File                        {#EnergyPlus}


Process flow for integrating with EnergyPlus

*	EnergyPlus starts, reads in the IDF, setups up the Simulation for interfacing with No-MASS
*	EnergyPlus extracts the NoMASS.fmu file
*	For each run period:
  *	Initialise No-MASS
  *	For each timestep:
    *	Set the variables in the No-MASS by passing an array of double values and an array of the value references given in the XML file
    *	Calls the No-MASS each step, No-MASS performs its calculations calculations
    *	Return the results of the No-MASS back to EnergyPlus, sends an array of the value references given in the XML file and the value of variable
  *	Terminates No-MASS
*	End of simulation
