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

The results file can be further filtered using regular expressions like that of below. Any output names that matches one of the options below is saved. If an output name does not match below it is discarded.

~~~~
<output>
  <regex>nsim</regex>
  <regex>TimeStep</regex>
  <regex>hour</regex>
  <regex>day</regex>
  <regex>Building[0-9]\d?_Appliance[0-9]\d?_received.*</regex>
  <regex>Building[0-9]\d?_Appliance1[0-9]\d?_received.*</regex>
  <regex>Building[0-9]\d?_Appliance10_supplied.*</regex>
  <regex>Building[0-9]\d?_Appliance10_supplied</regex>
  <regex>Building[0-9]\d?_Appliance1[0-9]\d?_supplied.*</regex>
  <regex>Building[0-9]\d?_Appliance1[0-9]\d?_received.*</regex>
  <regex>Building[0-9]\d?_Appliance[0-9]\d?_previous_state</regex>
  <regex>Building[0-9]\d?_Appliance[0-9]\d?_action</regex>
  <regex>Building[0-9]\d?_Appliance[0-9]\d?_reward</regex>
  <regex>Building[0-9]\d?_Appliance[0-9]\d?_state</regex>
  <regex>Building[0-9]\d?_Appliance1_cost</regex>
  <regex>Building[0-9]\d?_Appliance4_cost</regex>
  <regex>grid_power</regex>
  <regex>grid_cost</regex>
</output>
~~~~

## Q-Learning {#QL}
## Q-Learning for Occupants
Q-Learning occupant model specific however there will be times when you want to read from the learning data but not update the data with new values, for example once the training period is over.
Use learnupdate to turn this off.
The learn ep value is the epsilon value of Q-Learning and can be altered here with a double value.
~~~~
  <learnupdate>1</learnupdate>
  <learnep>0.01</learnep>
~~~~


## Grid cost {#QL}
## The cost of using the grid
Any appliance that using energy from the grid will use a tariff the is multiplied by the power used.
This is specified per day/hour/halfhour using an array with a length of 1/24/44 respectively. Each value is the cost of one unit of power.

<GridCost>0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.9, 0.9, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.9, 0.9, 0.9, 0.9, 0.9, 0.5, 0.5, 0.1 </GridCost>

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
* LargeCSV
* LargeLearning
* LargeLearningCSV
* Small
* PV
* CSV
* FMI
* Grid
* Battery
* BatteryGridCost

~~~~
<Appliances>
  <Large>...</Large>
  <LargeCSV>...</LargeCSV>
  <LargeLearning>...</LargeLearning>
  <LargeLearningCSV>...</LargeLearningCSV>
  <Small>...</Small>
  <pv>...</pv>
  <FMI>...</FMI>
  <Grid>...</Grid>
  <Battery>...</Battery>
  <BatteryGridCost>...</BatteryGridCost>
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

If required it is possible to override the large appliance predicted power profile with that of a profile specified through a csv file.

~~~~
<LargeCSV>
  <id>1</id>
  <priority>0.1,0.1,0.1,0.1,0.1,0.1,0.1, 0.1  ,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1</priority>
  <activities>WashingAppliance</activities>
  <fileProfile>profile1.csv</fileProfile>
</LargeCSV>
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

If required it is possible to override the large learning appliance predicted power profile with that of a profile specified through a csv file.

~~~~
<LargeLearningCSV>
  <id>4</id>
  <priority>0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1</priority>
  <timeRequired>0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1</timeRequired>
  <epsilon>0.1</epsilon>
  <alpha>0.3</alpha>
  <gamma>0.1</gamma>
  <updateQTable>1</updateQTable>
  <activities>WashingAppliance</activities>
  <fileProfile>profile4.csv</fileProfile>
</LargeLearningCSV>
~~~~

## Small Appliances {#AppliancesSmall}
## Small Appliances

The small appliance are defined through csv files included with NoMASS

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

## CSV Appliance {#AppliancesCSV}
## CSV Appliance
Filename is the CSV profile used to generate the supply/demand.
Cost is the how much the supplied power is.
~~~~
<csv>
    <id>11</id>
    <priority>100</priority>
    <demand>HeatingPower.csv</demand>
    <supply>turbineSupply.csv</supply>
    <cost>0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.0,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2</cost>
</csv>
~~~~


## FMI Appliance {#AppliancesFMI}
## FMI Appliance

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

## Battery {#AppliancesBattery}
## Battery

The Battery can learn when discharge it stored energy. This is achieved through the q-learning algorithm. With the battery it is possible to define when it can charge and discharge to the local neighbourhood.

~~~~
<battery>
  <id>12</id>
  <priority>0</priority>
  <epsilon>0.1</epsilon>
  <alpha>0.3</alpha>
  <gamma>0.1</gamma>
  <updateQTable>1</updateQTable>
  <batteryneighbourhooddischarge>0</batteryneighbourhooddischarge>
  <batteryneighbourhoodcharge>1</batteryneighbourhoodcharge>
</battery>
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

## models {#models}
## models

External to the the building tag are the model tags these specify the possible window or shading models to use. As defined on a per agent basis using ids.
~~~~
<models>
  ...
  <shades>
    <enabled>0</enabled>
    <shade>
    <id>1</id>
    ...
    </shade>
    ...
  </shades>
  <windows>
    <enabled>0</enabled>
    <window>
    <id>1</id>
    ...
    </window>
    ...
  </windows>
  ...
</models>
~~~~

## shades {#shades}
## shades

These are the coefficients as defined by the shading model

~~~~
<shade>
  <id>23</id>
  <name>204-10</name>
  <a01arr>-6.17</a01arr>
  <b01inarr>0.00114</b01inarr>
  <b01sarr>2.17</b01sarr>
  <a10arr>1.2</a10arr>
  <b10inarr>-0.00674</b10inarr>
  <b10sarr>-3.139</b10sarr>
  <a01int>-7.67</a01int>
  <b01inint>0.00088</b01inint>
  <b01sint>1.27</b01sint>
  <a10int>-2.61</a10int>
  <b10inint>-0.0051</b10inint>
  <b10sint>-2.683</b10sint>
  <afullraise>0.435</afullraise>
  <boutfullraise>1.95</boutfullraise>
  <bsfullraise>-0.0000231</bsfullraise>
  <bsfulllower>0.00000091</bsfulllower>
  <boutfulllower>-2.23</boutfulllower>
  <afulllower>-0.27</afulllower>
  <aSFlower>-2.294</aSFlower>
  <bSFlower>1.522</bSFlower>
  <shapelower>1.708</shapelower>
</shade>
~~~~


## shades {#shades}
## shades

These are the coefficients as defined by the window model.

~~~~
<window>
  <id>1</id>
  <aop>2.151</aop>
  <bopout>0.172</bopout>
  <shapeop>0.418</shapeop>
  <a01arr>-13.88</a01arr>
  <b01inarr>0.312</b01inarr>
  <b01outarr>0.0433</b01outarr>
  <b01absprevarr>1.862</b01absprevarr>
  <b01rnarr>-0.45</b01rnarr>
  <a01int>-12.23</a01int>
  <b01inint>0.281</b01inint>
  <b01outint>0.0271</b01outint>
  <b01presint>-0.000878</b01presint>
  <b01rnint>-0.336</b01rnint>
  <a01dep>-8.75</a01dep>
  <b01outdep>0.1371</b01outdep>
  <b01absdep>0.84</b01absdep>
  <b01gddep>0.83</b01gddep>
  <a10dep>-8.54</a10dep>
  <b10indep>0.213</b10indep>
  <b10outdep>-0.0911</b10outdep>
  <b10absdep>1.614</b10absdep>
  <b10gddep>-0.923</b10gddep>
 </window>
~~~~
