# Functional Mockup Unit (FMU)                       {#FMU}


A Functional Mockup Unit is a program that allows EnergyPlus to couple new custom functionality to EnergyPlus. It is a generic method for allowing simulation programs to be extended.
When applied information can be passed from EnergyPlus at runtime to a FMU which can then make calculations and send the results back to EnergyPlus. These results can then be used in EnergyPlus future calculations.
For more information on FMUs please see the [EnergyPlus website](http://nrel.github.io/EnergyPlus/ExternalInterfaces_Application_Guide/ExternalInterfaces_Application_Guide/#coupling-energyplus-with-functional-mock-up-units-for-co-simulation)

##What is a FMU

An FMU consists of three main parts:
* FMU Library (Windows DLL)
* Model Description File
* IDF Code used to enable the FMU

##How does an FMU Work

An FMU works using a simple data exchange methodology. At each timestep EnergyPlus sends a set of predefined variables (x) to the FMU (e.g. Zone Mean Air Temperature and/or Site Rain status...). The FMU performs a calculation on the values and returns a set of results (y) to EnergyPlus (e.g. occupant location and/or shade status...). These return values can be made to overwrite EnergyPlus values for the next timestep.

@image latex image7
@image html image7.png

A FMU can be developed to simulate heating setpoints, HVAC controllers or extend EnergyPlus to include any missing functionality.

##Linking to EnergyPlus

To run a simulation you will also need to run EnergyPlus with the idf file, epw file and the FMU in the same directory

##Model description

The model description tab allows you to specify the variables that will be passed between the FMU and EnergyPlus. EnergyPlus and the FMU use this to know what values they should expect and what they should send in return.
This is written in XML format, see below.

@image latex image1
@image html image1.png

The name of the the FMU(1) and the name of the model(2) need to be kept consistent throughout the dialog and should be provided with the imported FMU. The GUID(2) is a unique string that will also be provided with the FMU if required.
The variables to be pass are specified between items 4 and 11. A variable is defined with items 5 to 10. The start of the variable contains the the name associated to it, a unique number and direction the value will be sent. All EnergyPlus variables are of “Real” value so the type of variable will always be real.
The name of the variable will correspond to the value given in IDF value under FMU Model Variable Name definition, these should be identical. The value reference should always be unique to the variable and is used by EnergyPlus and the FMU to keep track of the values passed.
The causality of the variable specifies the direction the value of the variable will be sent. Shown below:

@image latex image8
@image html image8.png

Using this XML file you will also have to define in the IDF file which values EnergyPlus needs to send.

##IDF Script

The IDF script contents for FMU is explained in the EnergyPlus documentation, see [here](http://nrel.github.io/EnergyPlus/ExternalInterfaces_Application_Guide/ExternalInterfaces_Application_Guide/#fmu-examples)

The IDF FMU details tell EnergyPlus the values that it needs to know in order to run the FMU program.
First we define a ExternalInterface:FunctionalMockupUnitImport which specifies the file name of the FMU in the EnergyPlus folder. This is the name of the FMU plus the ”.fmu” file extension.

@image latex image6
@image html image6.png

Next we define all the variables that EnergyPlus will send to the FMU as (2). This includes the variable name and key (3), the name of the FMU(4), the model name (5), and the FMU variable name (6). The values for 5 & 6 need to be kept consistent with the values given in the corresponding variables in the model description file.

Finally there is the returned value from the FMU, in this case the shading value as an actuator.
These are the available external types that can be written to:
* ExternalInterface:FunctionalMockupUnitImport:To:Variable
* ExternalInterface:FunctionalMockupUnitImport:To:Schedule
* ExternalInterface:FunctionalMockupUnitImport:To:Actuator


##Developing a FMU

Developing an FMU requires some advance computer science knowledge, first how to program and second how to build that program into a library exported as a DLL in Microsoft Windows.
More information about developing FMUs can be found on the [Functional Mockup Interface website](https://www.fmi-standard.org/).

The basic flow for an FMU is as followed:

@image latex image3
@image html image3.png

* EnergyPlus starts, reads in the IDF, setups up the Simulation for FMU usage
* EnergyPlus extracts the .fmu file
* For each run period:
  * Initialise the FMU model by calling the initialise method
  * For each timestep:
    * Set the variables in the FMU by passing an array of double values and an array of the value references given in the XML file
    * Calls the FMU do step method, the fmu does any calculations
    * Return the results of the FMU back to EnergyPlus, sends a preallocated array that is written to and an array of the value references given in the XML file
  * Terminate the FMU by calling the terminate method
* End of simulation

##FMU file format

A fmu file such as agentFMU.fmu is just a zipped folder renamed with the from agentFMU.zip to agentFMU.fmu. The zipped folder has the following folder format:

@image latex image4
@image html image4.png

EnergyPlus extracts these files so the model description file can be edited to work with different buildings and configurations.
To Debug the DLL with EnergyPlus for testing this format will need to be used. Copy the DLL to the correct binaries folder, zip the folder and copy the renamed fmu to the EnergyPlus folder. Have Visual Studio run EnergyPlus, If everything is correctly setup then Visual Studio will be able to stop the program at breakpoints for inspection.
