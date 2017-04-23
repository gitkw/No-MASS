

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

configure_file(source/Gen.hpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/Gen.hpp)

set(gen_file "${CMAKE_CURRENT_BINARY_DIR}/Gen.hpp")

SET( TESTSRC
  ${gen_file}
  source/unittest_Simulation.cpp
  source/unittest_Configuration.cpp
  source/unittest_SimulationTime.cpp
  source/unittest_Model_Activity.cpp
  source/unittest_Model_Windows.cpp
  source/unittest_Occupant_Action_HeatingSetPoints_Learning.cpp
  source/unittest_Model_Appliance_Large_Usage.cpp
  source/unittest_Model_Appliance_Ownership.cpp
  source/unittest_Model_Appliance_Small_Usage.cpp
  source/unittest_StateMachine.cpp
  source/unittest_Log.cpp
  source/unittest_Building_Zone.cpp
  source/unittest_Utility.cpp
  source/unittest_Building.cpp
  source/unittest_Occupant.cpp
  source/unittest_Occupant_Zone.cpp
  source/unittest_Occupant_Action.cpp
  source/unittest_Occupant_Action_Window_Stochastic.cpp
  source/unittest_Occupant_Action_Window_Stochastic_BDI.cpp
  source/unittest_Occupant_Action_Shades.cpp
  source/unittest_Occupant_Action_Shades_BDI.cpp
  source/unittest_Occupant_Action_Light.cpp
  source/unittest_Occupant_Action_Light_BDI.cpp
  source/unittest_Occupant_Action_Window_Learning.cpp
  source/unittest_Occupant_Action_Heat_Gains.cpp
  source/unittest_QLearning_HeatingSetPoints.cpp
  source/unittest_QLearning_Window.cpp
  source/unittest_Appliance_Large.cpp
  source/unittest_Appliance_Large_Learning.cpp
  source/unittest_Appliance_Battery.cpp
  source/unittest_Appliance_Group_Large.cpp
  source/unittest_Appliance_Group_Battery.cpp
  source/unittest_Appliance_Group_CSV.cpp
  source/unittest_Appliance_Generic_CSV.cpp
  source/unittest_Building_Appliances.cpp
  source/unittest_Contract_Negotiation.cpp
  source/unittest_Contract_Node_Supply.cpp
  source/unittest_Contract_Node_Priority.cpp
  source/unittest_DataStore.cpp
)

add_executable(runUnitTests ${TESTSRC})

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)

# Extra linking for the project.
target_link_libraries(runUnitTests FMI)

add_test(runUnitTests runUnitTests)
