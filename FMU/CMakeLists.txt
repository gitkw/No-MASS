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
Source/Agent.h
Source/Agent_Action.cpp
Source/Agent_Action.h
Source/Agent_Action_Heat_Gains.cpp
Source/Agent_Action_Heat_Gains.h
Source/Agent_Action_Lights.cpp
Source/Agent_Action_Lights.h
Source/Agent_Action_Shades.cpp
Source/Agent_Action_Shades.h
Source/Agent_Action_Window.cpp
Source/Agent_Action_Window.h
Source/DataStore.cpp
Source/DataStore.h
Source/EP_IDFObject.cpp
Source/EP_IDFObject.h
Source/EP_ReadIDF.cpp
Source/EP_ReadIDF.h
Source/Log.cpp
Source/Log.h
Source/Model_Activity.cpp
Source/Model_Activity.h
Source/Model_Agents.cpp
Source/Model_Agents.h
Source/Model_EnergyPlus.cpp
Source/Model_EnergyPlus.h
Source/Model_ExternalShading.cpp
Source/Model_ExternalShading.h
Source/Model_HeatGains.cpp
Source/Model_HeatGains.h
Source/Model_Lights.cpp
Source/Model_Lights.h
Source/Model_Presence.cpp
Source/Model_Presence.h
Source/Model_RandomWeibull.cpp
Source/Model_RandomWeibull.h
Source/Model_Windows.cpp
Source/Model_Windows.h
Source/Model_ExternalShading.cpp
Source/Model_ExternalShading.h
Source/Model_Appliance_Ownership.cpp
Source/Model_Appliance_Ownership.h
Source/Model_Appliance_Usage.cpp
Source/Model_Appliance_Usage.h
Source/Simulation.cpp
Source/Simulation.h
Source/SimulationConfig.cpp
Source/SimulationConfig.h
Source/State.cpp
Source/State.h
Source/StateMachine.cpp
Source/StateMachine.h
Source/State_Audio_Visual.cpp
Source/State_Audio_Visual.h
Source/State_Cleaning.cpp
Source/State_Cleaning.h
Source/State_Cooking.cpp
Source/State_Cooking.h
Source/State_IT.cpp
Source/State_IT.h
Source/State_Metabolic.cpp
Source/State_Metabolic.h
Source/State_Out.cpp
Source/State_Out.h
Source/State_Passive.cpp
Source/State_Passive.h
Source/State_Present.cpp
Source/State_Present.h
Source/State_Sleep.cpp
Source/State_Sleep.h
Source/State_Washing.cpp
Source/State_Washing.h
Source/State_Washing_Appliance.cpp
Source/State_Washing_Appliance.h
Source/Utility.cpp
Source/Utility.h
Source/Zone.cpp
Source/Zone.h
Source/fmiFunctions.cpp
Source/fmiFunctions.h
Source/fmiPlatformTypes.h
Source/fmuTemplate.cpp
Source/fmuTemplate.h
)


include_directories(${PROJECT_SOURCE_DIR}/../Boost/Boost-Source)
include_directories(${PROJECT_SOURCE_DIR}/Source)

ADD_LIBRARY(FMI SHARED ${SRC})


if(WIN32)
  SET( CMAKE_CXX_CREATE_SHARED_LIBRARY  "${CMAKE_CXX_CREATE_SHARED_LIBRARY} -static -fPIC" )
endif()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2")
string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

SET_TARGET_PROPERTIES(FMI PROPERTIES
 CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror"
)

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

  SET( TESTSRC
    tests/source/unittest_Simulation.cc
    tests/source/unittest_SimulationConfig.cc
    tests/source/unittest_Model_Activity.cc
    tests/source/unittest_Model_HeatGains.cc
    tests/source/unittest_Model_Appliance_Usage.cc
    tests/source/unittest_Model_Appliance_Ownership.cc
    tests/source/unittest_SimulationConfig.cc
    tests/source/unittest_StateMachine.cc
    tests/source/unittest_Log.cc
    tests/source/unittest_Agent.cc
  )

  add_executable(runUnitTests ${TESTSRC})

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(runUnitTests FMI)

  add_test(runUnitTests runUnitTests)
endif()
