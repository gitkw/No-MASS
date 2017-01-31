# Simulation Configuration                        {#SimulationConfig}

[TOC]

This is an XML document that is used to inform No-MASS how to run.

It contains the information for the simulation period, buildings, zones, agents and appliances.

The initial tag to define a number simulation
~~~~
<simulation>
...
</simulation>
~~~~

## Seed {#Seed}
## Seed
As we use stochastic models we define a seed value. This can be removed and No-MASS will generate its own. However as it is often required for results can be repeated, doing it manully will allow the random values to be the same.

~~~~
  <seed>0</seed>
~~~~

## Time Period {#TP}
## Time Period
Here we define the time period information.
~~~~
  <timeStepsPerHour>60</timeStepsPerHour>
  <beginMonth>1</beginMonth>
  <endMonth>1</endMonth>
  <beginDay>1</beginDay>
  <endDay>7</endDay>
~~~~

## Saving Results {#SR}
## Saving Results
To save the results enable the save tag with the value 1. This writes out the No-MASS results to a NoMASS.out file. However this can slow the simulation down and can therefore be disabled with a 0 value.
~~~~
  <save>1</save>
~~~~

## Q-Learning {#QL}
## Q-Learning
Q-Learning is model specific however there will be times when you want to read from the learning data but not update the data with new values, for example once the training period is over.
Use learnupdate to turn this off.
The learn ep value is the epsilon value of Q-Learning and can be altered here with a double value.
~~~~
  <learnupdate>1</learnupdate>
  <learnep>0.01</learnep>
~~~~


## Buildings {#Buildings}
## Buildings
Buildings are defined within the buildings tag, like so.
~~~~
  <buildings>
    <building>
        ...
    </building>
        ...
    <building>
        ...
    </building>
  </buildings>
~~~~


## Building {#Building}
## Building

A Building is define with a set of zones, agents and appliances.

~~~~
<building>
      <zone>...</zone>
      <zone>...</zone>
      <Appliances>...</Appliances>
      <agents>...</agents>
</building>
~~~~

## Zone (Rooms) {#zone}
## Zone (Rooms)

A zone is a section of building often there is one per room, although a room can be sub divide into individual zones. Each zone is given a name, a set of activities and the number of windows in the the zone.

There are 10 activities as defined by the activity model:
* Cooking
* Cleaning
* AudioVisual
* Metabolic
* Passive
* Washing
* WashingAppliance
* Sleep
* IT
* Out

These are given to a zone and are comma seperated like below, however Out does not need to be defined.

The activities assign an agent to a zone, for example if Cooking is defined as an activity for that zone when an agent is in the state cooking they are also defined to this zone.

~~~~
<zone>
    <name>Block1:Kitchen</name>
    <activities>Cooking,Cleaning</activities>
    <windowCount>1</windowCount>
</zone>
<zone>
    <name>Block1:LivingRoom</name>
    <activities>AudioVisual,Metabolic,Passive</activities>
    <windowCount>1</windowCount>
</zone>
~~~~


## Appliances {#Appliances}
## Appliances

There are the following types of appliance:

* Large
* LargeLearning
* Small
* PV
* FMI
* Grid

~~~~
<Appliances>
  <Large>...</Large>
  <LargeLearning>...</LargeLearning>
  <Small>...</Small>
  <pv>...</pv>
  <FMI>...</FMI>
  <Grid>...</Grid>
</Appliances>
~~~~

### Appliance Priority {#Appliancespriority}
### Appliance Priority

Each Appliance is given a priority, which is a value that defines the order in which the device will receive its demanded energy, higher priority energy receive there demand first.

## Large Appliances {#AppliancesLarge}
## Large Appliances

The large appliance has a id that relates to the id of the large appliances defined in the ApplianceLarge.xml file.

~~~~
<Large>
  <id>0</id>
  <priority>0</priority>
</Large>
~~~~

The large appliance can also be given a set of activities meaning that the appliance will only turn on when an agent is in the activity specified.

~~~~
<Large>
  <id>0</id>
  <priority>0</priority>
  <activities>Cooking,Cleaning</activities>
</Large>
~~~~


## Large Appliances Q-Learning {#AppliancesLargeLearning}
## Large Appliances Q-Learning

Define in the same way as a large appliance only now we use the tag LargeLearning instead. This allows the applaince to shift its profile over time. Using the Q-Learning algorithm hopefully to a time when the supplied power is cheap.

~~~~
<LargeLearning>
  <id>1</id>
  <priority>0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1</priority>
  <timeRequired>0.1,0.1,0.1,0.1,0.1,0.1,0.1,1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1</timeRequired>
  <epsilon>0.1</epsilon>
  <alpha>0.3</alpha>
  <gamma>0.1</gamma>
  <updateQTable>1</updateQTable>
  <activities>WashingAppliance</activities>
</LargeLearning>
~~~~

## Small Appliances {#AppliancesSmall}
## Small Appliances

~~~~
<Small>
  <id>6</id>
  <priority>6</priority>
  <WeibullParameters>weibull_parameters_audiovisual.csv</WeibullParameters>
  <StateProbabilities>state_probability_audiovisual.csv</StateProbabilities>
  <Fractions>mean_fractional_audiovisual.csv</Fractions>
  <SumRatedPowers>sum_rated_av.txt</SumRatedPowers>
</Small>
~~~~

## PV {#AppliancesPV}
## PV
Filename is the CSV profile used to generate the supply.
Cost is the how much the supplied power is.

~~~~
      <pv>
        <id>10</id>
        <priority>0</priority>
        <filename>PVBowler2013_365.csv</filename>
        <cost>0.02</cost>
      </pv>
~~~~

## FMI {#AppliancesFMI}
## FMI

Only used for appliances from the FMI interface (such as EnergyPlus sending the power), the variable name needs to correspond with the name in the modeldescription.xml file.

~~~~
      <FMI>
        <id>11</id>
        <priority>0</priority>
        <variablename>HVACPower</variablename>
      </FMI>
~~~~

## Grid {#AppliancesGrid}
## Grid

Calculate the supply needed based on supply of the other appliances minus the demand of the other appliances.
Cost is the how much the supplied power is.

~~~~
      <Grid>
        <id>1000</id>
        <priority>0</priority>
        <cost>1.00</cost>
      </Grid>
~~~~

## Agents {#Agents}
## Agents
The definition of an agent. The window and shade parameter is the id of the window and shade profile that the agent uses.
Office and bedroom are the corresponding locations for acticities IT and Sleep.
Profile is the activity profile to use, in the example we specify the file used.
The other parameters correspond to the activity model.

~~~~

<agents>
    ...
      <agent>
        <shade>1</shade>
        <window>1</window>
        <office>Block1:Zone1</office>
        <power>0.5</power>
        <age>age2</age>
        <computer>computer0</computer>
        <civstat>civstat1</civstat>
        <unemp>unemp0</unemp>
        <retired>retired1</retired>
        <edtry>edtry1</edtry>
        <famstat>famstat3</famstat>
        <sex>sex2</sex>
        <bedroom>Block2:MasterBedroom</bedroom>
        <profile>
          <file>Activity.xml</file>
        </profile>
      </agent>
      ...
</agents>
~~~~
