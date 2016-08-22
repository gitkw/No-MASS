# Model Description                        {#ModelDescription}

[TOC]

The model description tab allows you to specify the variables that will be passed between the FMU and EnergyPlus. EnergyPlus and the FMU use this to know what values they should expect and what they should send in return.

This is written in XML format, see below.

~~~~
<fmiModelDescription description="Model with interfaces for media with moist air that will be linked to the BCVTB which models the response of the room" fmiVersion="1.0" generationDateAndTime="2012-04-17T19:12:58Z" generationTool="Dymola Version 2012 FD01 (32-bit), 2011-11-22" guid="{fd719ef5-c46e-48c7-ae95-96089a69ee64}" modelIdentifier="FMI" modelName="FMI" numberOfContinuousStates="0" numberOfEventIndicators="0" variableNamingConvention="structured" version="1.2">
  <TypeDefinitions>
    <Type name="Modelica.Blocks.Interfaces.RealInput">
      <RealType />
    </Type>
    <Type name="Modelica.Blocks.Interfaces.RealOutput">
      <RealType />
    </Type>
  </TypeDefinitions>
  <DefaultExperiment startTime="0.0" stopTime="1.0" tolerance="1E-005" />
  <ModelVariables>
    <ScalarVariable causality="input" name="Block1:Zone1ZoneAirRelativeHumidity" valueReference="1">
      <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
    </ScalarVariable>
    <ScalarVariable causality="input" name="Block1:Zone1ZoneMeanRadiantTemperature" valueReference="2">
      <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
    </ScalarVariable>
    <ScalarVariable causality="output" name="Block1:Zone1BlindFraction" valueReference="3">
      <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="1.0" />
    </ScalarVariable>
  ...
  </ModelVariables>
</fmiModelDescription>
~~~~

## ScalarVariable {#ScalarVariable}
## ScalarVariable

A ScalarVariable declares the parameter that will be passed

### Name {#Name}
### Name

The name of the variable gives the program understanding as to the parameter being passed, NoMass expects these to be declared in a specific way depending on how they are used by a model.

### ValueReference {#ValueReference}
### ValueReference

The value reference should be unique and links the variable in the code to the specific value.
For example, the name will be linked to a the value reference so when the variable is passed from the main program it is passed with its value reference. With this information the variable is known.

### Causality {#Causality}
### Causality

The causality of the variable specifies the direction the value of the variable will be sent.

* Input is into the FMU
* Output is out of the FMU into the main program

### Real {#Real}
### Real

For NoMASS the variable is always  a real.

#### declaredType {#declaredType}
#### declaredType

The declared type is always Modelica.Blocks.Interfaces.RealInput.

#### start {#start}
#### start

Start is the initial value the variable will have.
