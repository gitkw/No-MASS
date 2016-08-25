cmake_minimum_required (VERSION 2.6)


################################
# Options
################################

option(Tests "Tests" OFF)
option(CrossCompile32 "CrossCompile32" OFF)
option(CrossCompile64 "CrossCompile64" OFF)

################################
# Cross Compiling
################################

if(CrossCompile32 OR CrossCompile64)
  SET(CMAKE_SYSTEM_NAME Windows)
  set(COMPILER_PREFIX "i686-w64-mingw32")
  if(CrossCompile64)
    set(COMPILER_PREFIX "x86_64-w64-mingw32")
  endif()
  find_program(CMAKE_RC_COMPILER NAMES ${COMPILER_PREFIX}-windres)
  SET(CMAKE_RC_COMPILER ${COMPILER_PREFIX}-windres)
  find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
  SET(CMAKE_C_COMPILER ${COMPILER_PREFIX}-gcc)
  find_program(CMAKE_CXX_COMPILER NAMES ${COMPILER_PREFIX}-g++)
  SET(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}-g++)

  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

endif()

################################
# Project
################################

project (FMI)

SET( SRC
Source/Agent.cpp
Source/Occupant.cpp
Source/Occupant_Action.cpp
Source/Occupant_Action_Heat_Gains.cpp
Source/Occupant_Action_Lights.cpp
Source/Occupant_Action_Lights_BDI.cpp
Source/Occupant_Action_Shades.cpp
Source/Occupant_Action_Shades_BDI.cpp
Source/Occupant_Action_Window.cpp
Source/Occupant_Action_Window_Stochastic.cpp
Source/Occupant_Action_Window_Stochastic_BDI.cpp
Source/Occupant_Action_Window_Learning.cpp
Source/Occupant_Action_Appliance.cpp
Source/Occupant_Action_Appliance_BDI.cpp
Source/Occupant_Action_HeatingSetPoints_Learning.cpp
Source/Occupant_Zone.cpp
Source/DataStore.cpp
Source/Log.cpp
Source/Model_Activity.cpp
Source/Model_Activity_Survival.cpp
Source/Model_ExternalShading.cpp
Source/Model_HeatGains.cpp
Source/Model_Lights.cpp
Source/Model_Presence.cpp
Source/Model_RandomWeibull.cpp
Source/Model_Windows.cpp
Source/Model_Appliance_Ownership.cpp
Source/Model_Appliance_Large_Usage.cpp
Source/Model_Appliance_Large_Usage_Survival.cpp
Source/Model_Appliance_Small_Usage.cpp
Source/Model_Appliance_PV_CSV.cpp
Source/Simulation.cpp
Source/SimulationConfig.cpp
Source/State.cpp
Source/StateMachine.cpp
Source/State_Audio_Visual.cpp
Source/State_Cleaning.cpp
Source/State_Cooking.cpp
Source/State_IT.cpp
Source/State_Metabolic.cpp
Source/State_Out.cpp
Source/State_Passive.cpp
Source/State_Present.cpp
Source/State_Sleep.cpp
Source/State_Washing.cpp
Source/State_Washing_Appliance.cpp
Source/Utility.cpp
Source/Environment.cpp
Source/Building.cpp
Source/Building_Zone.cpp
Source/Building_Appliances.cpp
Source/fmiFunctions.cpp
Source/QLearning.cpp
Source/QLearning_HeatingSetPoints.cpp
Source/QLearning_Window.cpp
Source/QLearning_Appliance.cpp
Source/Appliance.cpp
Source/Appliance_Large.cpp
Source/Appliance_Small.cpp
Source/Appliance_PV.cpp
Source/Appliance_FMI.cpp
Source/Appliance_Large_Learning.cpp
Source/LVN.cpp
Source/LVN_Node.cpp
Source/LVN_Negotiation.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/../rapidxml)
include_directories(${PROJECT_SOURCE_DIR}/Source)

ADD_LIBRARY(NoMASSstatic STATIC ${SRC})

ADD_LIBRARY(FMI SHARED Source/fmuTemplate.cpp)
target_link_libraries(FMI NoMASSstatic)

add_executable(NoMASS Source/main.cpp)
target_link_libraries(NoMASS NoMASSstatic)

if(UNIX)
  set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "-Werror -Wall -Wextra")
endif()

if(WIN32)
  SET( CMAKE_CXX_CREATE_SHARED_LIBRARY  "${CMAKE_CXX_CREATE_SHARED_LIBRARY} -static -fPIC" )
endif()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")


SET(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2")
string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")


include_directories("${CMAKE_CURRENT_BINARY_DIR}")
set(testLocationFiles "${PROJECT_SOURCE_DIR}/tests/Files")

################################
# Testing
################################
if (Tests)


  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread" )
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(tests/googletest/googletest/)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############

  configure_file(tests/source/Gen.h.in
                  ${CMAKE_CURRENT_BINARY_DIR}/Gen.h)

  if(CMAKE_GENERATOR STREQUAL Xcode)
    configure_file(tests/Files/SimulationConfig1.xml.in
                    ${CMAKE_CURRENT_BINARY_DIR}/Debug/SimulationConfig1.xml)
  endif()

  configure_file(tests/Files/SimulationConfig1.xml.in
                  ${CMAKE_CURRENT_BINARY_DIR}/SimulationConfig1.xml)
  set(gen_file "${CMAKE_CURRENT_BINARY_DIR}/Gen.h")

  SET( TESTSRC
    ${gen_file}
    tests/source/unittest_Simulation.cc
    tests/source/unittest_SimulationConfig.cc
    tests/source/unittest_Model_Activity.cc
    tests/source/unittest_Model_Windows.cc
    tests/source/unittest_Occupant_Action_HeatingSetPoints_Learning.cc
    tests/source/unittest_Model_Appliance_Large_Usage.cc
    tests/source/unittest_Model_Appliance_Ownership.cc
    tests/source/unittest_Model_Appliance_Small_Usage.cc
    tests/source/unittest_StateMachine.cc
    tests/source/unittest_Log.cc
    tests/source/unittest_Building_Zone.cc
    tests/source/unittest_Utility.cc
    tests/source/unittest_Building.cc
    tests/source/unittest_Occupant.cc
    tests/source/unittest_Occupant_Zone.cc
    tests/source/unittest_Occupant_Action.cc
    tests/source/unittest_Occupant_Action_Window_Stochastic.cc
    tests/source/unittest_Occupant_Action_Window_Stochastic_BDI.cc
    tests/source/unittest_Occupant_Action_Shades.cc
    tests/source/unittest_Occupant_Action_Shades_BDI.cc
    tests/source/unittest_Occupant_Action_Light.cc
    tests/source/unittest_Occupant_Action_Light_BDI.cc
    tests/source/unittest_Occupant_Action_Window_Learning.cc
    tests/source/unittest_Occupant_Action_Heat_Gains.cc
    tests/source/unittest_QLearning_HeatingSetPoints.cc
    tests/source/unittest_QLearning_Window.cc
    tests/source/unittest_Appliance_Large.cc
    tests/source/unittest_Building_Appliances.cc
  )

  add_executable(runUnitTests ${TESTSRC})

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(runUnitTests FMI)

  add_test(runUnitTests runUnitTests)
endif()

configure_file(tests/Files/DSM.xml
                ${CMAKE_CURRENT_BINARY_DIR}/SimulationConfig.xml)
configure_file(tests/Files/DSM1house.xml
                ${CMAKE_CURRENT_BINARY_DIR}/DSM1house.xml)
configure_file(tests/Files/DSM2houses.xml
                ${CMAKE_CURRENT_BINARY_DIR}/DSM2houses.xml)
configure_file(tests/Files/DSM10houses.xml
                ${CMAKE_CURRENT_BINARY_DIR}/DSM10houses.xml)
configure_file(tests/Files/Activity.xml
                ${CMAKE_CURRENT_BINARY_DIR}/Activity.xml)
configure_file(tests/Files/AppliancesLarge.xml
                ${CMAKE_CURRENT_BINARY_DIR}/AppliancesLarge.xml)
configure_file(tests/Files/PVBowler2013_365.csv
                ${CMAKE_CURRENT_BINARY_DIR}/PVBowler2013_365.csv)


file(COPY tests/Files/SmallAppliances DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
