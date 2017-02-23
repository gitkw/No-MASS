

set(testLocationFiles "${PROJECT_SOURCE_DIR}/tests/Files/")

SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread" )
# This adds another subdirectory, which has 'project(gtest)'.
add_subdirectory(googletest/googletest/)

enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

##############
# Unit Tests
##############

configure_file(source/Gen.h.in
                ${CMAKE_CURRENT_BINARY_DIR}/Gen.h)

set(gen_file "${CMAKE_CURRENT_BINARY_DIR}/Gen.h")

SET( TESTSRC
  ${gen_file}
  source/unittest_Simulation.cc
  source/unittest_SimulationConfig.cc
  source/unittest_SimulationTime.cc
  source/unittest_Model_Activity.cc
  source/unittest_Model_Windows.cc
  source/unittest_Occupant_Action_HeatingSetPoints_Learning.cc
  source/unittest_Model_Appliance_Large_Usage.cc
  source/unittest_Model_Appliance_Ownership.cc
  source/unittest_Model_Appliance_Small_Usage.cc
  source/unittest_StateMachine.cc
  source/unittest_Log.cc
  source/unittest_Building_Zone.cc
  source/unittest_Utility.cc
  source/unittest_Building.cc
  source/unittest_Occupant.cc
  source/unittest_Occupant_Zone.cc
  source/unittest_Occupant_Action.cc
  source/unittest_Occupant_Action_Window_Stochastic.cc
  source/unittest_Occupant_Action_Window_Stochastic_BDI.cc
  source/unittest_Occupant_Action_Shades.cc
  source/unittest_Occupant_Action_Shades_BDI.cc
  source/unittest_Occupant_Action_Light.cc
  source/unittest_Occupant_Action_Light_BDI.cc
  source/unittest_Occupant_Action_Window_Learning.cc
  source/unittest_Occupant_Action_Heat_Gains.cc
  source/unittest_QLearning_HeatingSetPoints.cc
  source/unittest_QLearning_Window.cc
  source/unittest_Appliance_Large.cc
  source/unittest_Appliance_Group_Large.cc
  source/unittest_Building_Appliances.cc
  source/unittest_Contract_Negotiation.cc
  source/unittest_Contract_Node_Supply.cc
  source/unittest_Contract_Node_Priority.cc
  source/unittest_DataStore.cc
)

add_executable(runUnitTests ${TESTSRC})

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)

# Extra linking for the project.
target_link_libraries(runUnitTests FMI)

add_test(runUnitTests runUnitTests)
