# EnergyPlus Examples                       {#EnergyPlusExamples}


## Example 1: Shoe box configuration file

The following example enables No-MASS to run a non-residential building configuration.

* A building is defined
  * Two agents are working in the building and assigned to the only zone
  * Both agents have the same presence profile, have equal power and use the same shade and window model


### XML File
~~~~{.xml}
<?xml version="1.0" encoding="UTF-8"?>
<simulation>
  <seed>0</seed>
  <timeStepsPerHour>12</timeStepsPerHour>
  <beginMonth>1</beginMonth>
  <endMonth>12</endMonth>
  <beginDay>1</beginDay>
  <endDay>31</endDay>
  <learn>0</learn>
  <buildings>
  <building>
    <zone>
      <name>Block1:Zone1</name>
      <activities>IT</activities>
      <groundFloor>1</groundFloor>
      <windowCount>1</windowCount>
    </zone>
    <agents>
      <agent>
        <shade>1</shade>
        <window>1</window>
        <office>Block1:Zone1</office>
        <power>0.5</power>
        <profile>
          <monday>0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.025,0.250,0.422,0.309,0.377,0.187,0.375,0.426,0.396,0.375,0.432,0.084,0.070,0.047,0.039,0.038,0.038</monday>
          <tuesday>0.040,0.038,0.038,0.038,0.038,0.038,0.038,0.046,0.320,0.401,0.325,0.417,0.196,0.372,0.435,0.402,0.334,0.435,0.100,0.053,0.044,0.044,0.042,0.042</tuesday>
          <wednesday>0.041,0.041,0.041,0.041,0.041,0.041,0.041,0.062,0.342,0.403,0.297,0.342,0.194,0.354,0.395,0.363,0.336,0.383,0.080,0.043,0.027,0.026,0.026,0.026</wednesday>
          <thursday>0.024,0.024,0.024,0.024,0.024,0.024,0.024,0.029,0.265,0.373,0.271,0.368,0.193,0.361,0.402,0.357,0.324,0.367,0.094,0.067,0.032,0.032,0.030,0.030</thursday>
          <friday>0.030,0.029,0.029,0.029,0.029,0.029,0.029,0.055,0.327,0.367,0.304,0.373,0.215,0.317,0.280,0.239,0.197,0.125,0.066,0.069,0.031,0.029,0.027,0.027</friday>
          <saturday>0.028,0.030,0.029,0.029,0.030,0.029,0.029,0.030,0.030,0.030,0.035,0.037,0.030,0.032,0.041,0.045,0.042,0.034,0.035,0.032,0.027,0.024,0.021,0.021</saturday>
          <sunday>0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.024,0.025,0.032,0.045,0.040,0.041,0.033,0.032,0.033,0.031,0.031,0.028,0.026,0.027,0.025,0.025</sunday>
        </profile>
      </agent>
      <agent>
        <shade>1</shade>
        <window>1</window>
        <office>Block1:Zone1</office>
        <power>0.5</power>
        <profile>
          <monday>0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.025,0.250,0.422,0.309,0.377,0.187,0.375,0.426,0.396,0.375,0.432,0.084,0.070,0.047,0.039,0.038,0.038</monday>
          <tuesday>0.040,0.038,0.038,0.038,0.038,0.038,0.038,0.046,0.320,0.401,0.325,0.417,0.196,0.372,0.435,0.402,0.334,0.435,0.100,0.053,0.044,0.044,0.042,0.042</tuesday>
          <wednesday>0.041,0.041,0.041,0.041,0.041,0.041,0.041,0.062,0.342,0.403,0.297,0.342,0.194,0.354,0.395,0.363,0.336,0.383,0.080,0.043,0.027,0.026,0.026,0.026</wednesday>
          <thursday>0.024,0.024,0.024,0.024,0.024,0.024,0.024,0.029,0.265,0.373,0.271,0.368,0.193,0.361,0.402,0.357,0.324,0.367,0.094,0.067,0.032,0.032,0.030,0.030</thursday>
          <friday>0.030,0.029,0.029,0.029,0.029,0.029,0.029,0.055,0.327,0.367,0.304,0.373,0.215,0.317,0.280,0.239,0.197,0.125,0.066,0.069,0.031,0.029,0.027,0.027</friday>
          <saturday>0.028,0.030,0.029,0.029,0.030,0.029,0.029,0.030,0.030,0.030,0.035,0.037,0.030,0.032,0.041,0.045,0.042,0.034,0.035,0.032,0.027,0.024,0.021,0.021</saturday>
          <sunday>0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.021,0.024,0.025,0.032,0.045,0.040,0.041,0.033,0.032,0.033,0.031,0.031,0.028,0.026,0.027,0.025,0.025</sunday>
        </profile>
      </agent>
    </agents>
  </building>
  </buildings>
  <models>
    <presencePage>1</presencePage>
    <shades>
      <enabled>1</enabled>
      <shade>
      <id>1</id>
      <name>001-28-X</name>
      <a01arr>-6.39</a01arr>
      <b01inarr>0.00141</b01inarr>
      <b01sarr>2.17</b01sarr>
      <a10arr>-2.71</a10arr>
      <b10inarr>-0.00364</b10inarr>
      <b10sarr>-3.139</b10sarr>
      <a01int>-6.66</a01int>
      <b01inint>0.00086</b01inint>
      <b01sint>1.27</b01sint>
      <a10int>-3.87</a10int>
      <b10inint>-0.00358</b10inint>
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
    </shades>
    <windows>
      <enabled>1</enabled>
      <window>
      <id>1</id>
      <aop>2.151</aop>
      <bopout>0.172</bopout>
      <shapeop>0.418</shapeop>
      <a01arr>-13.70</a01arr>
      <b01inarr>0.308</b01inarr>
      <b01outarr>0.0395</b01outarr>
      <b01absprevarr>1.862</b01absprevarr>
      <b01rnarr>-0.43</b01rnarr>
      <a01int>-11.78</a01int>
      <b01inint>0.263</b01inint>
      <b01outint>0.0394</b01outint>
      <b01presint>-0.0009</b01presint>
      <b01rnint>-0.336</b01rnint>
      <a01dep>-8.72</a01dep>
      <b01outdep>0.1352</b01outdep>
      <b01absdep>0.85</b01absdep>
      <b01gddep>0.82</b01gddep>
      <a10dep>-8.68</a10dep>
      <b10indep>0.222</b10indep>
      <b10outdep>-0.0936</b10outdep>
      <b10absdep>1.534</b10absdep>
      <b10gddep>-0.845</b10gddep>
      </window>
      </windows>
    <lights>1</lights>
  </models>
</simulation>

~~~~

### IDF File

~~~~
Output:Variable,*,Schedule Value,Timestep;
Output:Variable,*,AFN Surface Venting Window or Door Opening Factor,timestep;
Output:Variable,*,Zone Exterior Windows Total Transmitted Beam Solar Radiation Rate,timestep;
Output:Variable,*,Window Shading Fraction,timestep;

Output:Variable,*,Zone People Radiant Heating Rate,timestep;
Output:Variable,*,Zone Air Relative Humidity,timestep;
Output:Variable,*,Zone Mean Radiant Temperature,timestep;
Output:Variable,*,Zone People Occupant Count,timestep;
Output:Variable,*,Daylighting Reference Point 1 Illuminance,timestep;
Output:Variable,*,Zone Mean Radiant Temperature,timestep;
Output:Variable,*,Site Exterior Horizontal Sky Illuminance,timestep;
Output:Variable,*,Site Rain Status,timestep;
Output:Variable,*,Site Outdoor Air Drybulb Temperature,timestep;
Output:Variable,*,Zone Lights Electric Power,Timestep;
Output:Variable,*,Zone Lights Electric Energy,Timestep;
Output:Variable,*,Zone Air System Sensible Heating Energy,Timestep;
Output:Variable,*,Zone Air System Sensible Heating Rate,Timestep;
Output:Variable,*,Zone Air System Sensible Cooling Energy,Timestep;
Output:Variable,*,Zone Air System Sensible Cooling Rate,Timestep;
Output:Variable,*,Zone Mean Air Temperature,Timestep;
Output:Variable, *, Zone Thermal Comfort Fanger Model PMV, Timestep;
Output:Variable, *, Zone Thermal Comfort Fanger Model PPD, Timestep;


Output:Variable,*,Zone People Radiant Heating Rate,monthly;
Output:Variable,*,Zone Air Relative Humidity,monthly;
Output:Variable,*,Zone Mean Radiant Temperature,monthly;
Output:Variable,*,Zone People Occupant Count,monthly;
Output:Variable,*,Daylighting Reference Point 1 Illuminance,monthly;
Output:Variable,*,Zone Mean Radiant Temperature,monthly;
Output:Variable,*,Site Exterior Horizontal Sky Illuminance,monthly;
Output:Variable,*,Site Rain Status,monthly;
Output:Variable,*,Site Outdoor Air Drybulb Temperature,monthly;
Output:Variable,*,Zone Lights Electric Power,monthly;
Output:Variable,*,Zone Lights Electric Energy,monthly;
Output:Variable,*,Zone Air System Sensible Heating Energy,monthly;
Output:Variable,*,Zone Air System Sensible Heating Rate,monthly;
Output:Variable,*,Zone Air System Sensible Cooling Energy,monthly;
Output:Variable,*,Zone Air System Sensible Cooling Rate,monthly;
Output:Variable,*,Zone Mean Air Temperature,monthly;

Output:Variable,*,Zone People Radiant Heating Rate,runperiod;
Output:Variable,*,Zone Air Relative Humidity,runperiod;
Output:Variable,*,Zone Mean Radiant Temperature,runperiod;
Output:Variable,*,Zone People Occupant Count,runperiod;
Output:Variable,*,Daylighting Reference Point 1 Illuminance,runperiod;
Output:Variable,*,Zone Mean Radiant Temperature,runperiod;
Output:Variable,*,Site Exterior Horizontal Sky Illuminance,runperiod;
Output:Variable,*,Site Rain Status,runperiod;
Output:Variable,*,Site Outdoor Air Drybulb Temperature,runperiod;
Output:Variable,*,Zone Lights Electric Power,runperiod;
Output:Variable,*,Zone Lights Electric Energy,runperiod;
Output:Variable,*,Zone Air System Sensible Heating Energy,runperiod;
Output:Variable,*,Zone Air System Sensible Heating Rate,runperiod;
Output:Variable,*,Zone Air System Sensible Cooling Energy,runperiod;
Output:Variable,*,Zone Air System Sensible Cooling Rate,runperiod;
Output:Variable,*,Zone Mean Air Temperature,runperiod;


ExternalInterface,
  FunctionalMockupUnitImport;                !- Name of External Interface


ExternalInterface:FunctionalMockupUnitImport,
  agentFMU.fmu,                              !- FMU Filename
   15,                                       !- FMU Timeout in milli-seconds
   0;                                        !- FMU LoggingOn Value


ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Block1:Zone1,                            !- EnergyPlus Key Value,
  Zone Air Relative Humidity,              !- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  Block1:Zone1ZoneAirRelativeHumidity;     !- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Block1:Zone1,                            !- EnergyPlus Key Value,
  Zone Mean Radiant Temperature,           !- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  Block1:Zone1ZoneMeanRadiantTemperature;  !- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Block1:Zone1,                            !- EnergyPlus Key Value,
  Zone Mean Air Temperature,               !- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  Block1:Zone1ZoneMeanAirTemperature;      !- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Block1:Zone1,                            !- EnergyPlus Key Value,
  Daylighting Reference Point 1 Illuminance,!- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  Block1:Zone1DaylightingReferencePoint1Illuminance;!- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:To:Schedule,
  Block1:Zone1NumberOfOccupants,           !- EnergyPlus Key Value,
  Any Number,                              !- Schedule Type Limits Names,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Model Name,
  Block1:Zone1NumberOfOccupants,           !- FMU Model Variable Name,
  0.0;                                     !- Initial Value

  ExternalInterface:FunctionalMockupUnitImport:To:Schedule,
    Block1:Zone1Heating,                     !- EnergyPlus Key Value,
    Any Number,                              !- Schedule Type Limits Names,
    agentFMU.fmu,                            !- FMU Filename,
    FMI,                                     !- FMU Model Name,
    Block1:Zone1Heating,                     !- FMU Model Variable Name,
    0.0;                                     !- Initial Value


ExternalInterface:FunctionalMockupUnitImport:To:Actuator,
  Block1:Zone1_Wall_5_0_0_0_0_0_Win_Shading_Fraction, !- EnergyPlus Variable Name
  Block1:Zone1_Wall_5_0_0_0_0_0_Win, !- Actuated Component Unique Name
  Window Shading Fraction, !- Actuated Component Type
  Control Fraction, !- Actuated Component Control Type
  agentFMU.fmu, !- FMU Filename
  FMI, !- FMU Model Name
  Block1:Zone1BlindFraction, !- FMU Model Variable Name
  1.0; !- Initial Value!

ExternalInterface:FunctionalMockupUnitImport:To:Schedule,
  Block1:Zone1LightState,                  !- EnergyPlus Key Value,
  Any Number,                              !- Schedule Type Limits Names,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Model Name,
  Block1:Zone1LightState,                  !- FMU Model Variable Name,
  0.0;                                     !- Initial Value

ExternalInterface:FunctionalMockupUnitImport:To:Schedule,
  Block1:Zone1WindowState0,                 !- EnergyPlus Key Value,
  Any Number,                              !- Schedule Type Limits Names,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Model Name,
  Block1:Zone1WindowState0,                 !- FMU Model Variable Name,
  0.0;                                     !- Initial Value


ExternalInterface:FunctionalMockupUnitImport:To:Schedule,
  Block1:Zone1AverageGains,                !- EnergyPlus Key Value,
  Any Number,                              !- Schedule Type Limits Names,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Model Name,
  Block1:Zone1AverageGains,                !- FMU Model Variable Name,
  0.0;                                     !- Initial Value

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Environment,                             !- EnergyPlus Key Value,
  Site Exterior Horizontal Sky Illuminance,!- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  EnvironmentSiteExteriorHorizontalSkyIlluminance;!- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Environment,                             !- EnergyPlus Key Value,
  Site Rain Status,                        !- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  EnvironmentSiteRainStatus;               !- FMU Variable Name

ExternalInterface:FunctionalMockupUnitImport:From:Variable,
  Environment,                             !- EnergyPlus Key Value,
  Site Outdoor Air Drybulb Temperature,    !- EnergyPlus Variable Name,
  agentFMU.fmu,                            !- FMU Filename,
  FMI,                                     !- FMU Instance Name,
  EnvironmentSiteOutdoorAirDrybulbTemperature;!- FMU Variable Name


  ~~~~

  ### Model Description File

  ~~~~{.xml}

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
      <ScalarVariable causality="input" name="Block1:Zone1ZoneMeanAirTemperature" valueReference="3">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="Block1:Zone1DaylightingReferencePoint1Illuminance" valueReference="4">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="output" name="Block1:Zone1NumberOfOccupants" valueReference="5">
        <Real declaredType="Modelica.Blocks.Interfaces.RealOutput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="output" name="Block1:Zone1AverageGains" valueReference="6">
        <Real declaredType="Modelica.Blocks.Interfaces.RealOutput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="EnvironmentSiteExteriorHorizontalSkyIlluminance" valueReference="7">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="EnvironmentSiteRainStatus" valueReference="8">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="EnvironmentSiteOutdoorAirDrybulbTemperature" valueReference="9">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="EMSwarmUpComplete" valueReference="10">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="input" name="EMSepTimeStep" valueReference="11">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="output" name="Block1:Zone1BlindFraction" valueReference="12">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="1.0" />
      </ScalarVariable>
      <ScalarVariable causality="output" name="Block1:Zone1WindowState0" valueReference="13">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
      <ScalarVariable causality="output" name="Block1:Zone1LightState" valueReference="14">
        <Real declaredType="Modelica.Blocks.Interfaces.RealInput" start="0.0" />
      </ScalarVariable>
    </ModelVariables>
  </fmiModelDescription>

  ~~~~
