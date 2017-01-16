// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "Gen.h"

#include "SimulationConfig.h"
#include "DataStore.h"

#include "gtest/gtest.h"

class Test_DataStore : public ::testing::Test {
 protected:
    int res;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_DataStore::SetUp() {
  DataStore::clear();
  SimulationConfig::outputRegexs.clear();
}

void Test_DataStore::AfterConfiguration() {
}

TEST_F(Test_DataStore, regex) {
  //SimulationConfig::outputRegexs.push_back("[a-zA-Z0-9\_]+");
  SimulationConfig::outputRegexs.push_back(".*");
  //EXPECT_EQ(SimulationConfig::outputRegexs.at(0), "*");

  res = DataStore::addVariable("helloWorld");
  EXPECT_EQ(res, 0);
  res = DataStore::addVariable("hello1World");
  EXPECT_EQ(res, 1);
  res = DataStore::addVariable("hello2_World");
  EXPECT_EQ(res, 2);
  res = DataStore::addVariable("hello2_World");
  EXPECT_EQ(res, 2);
  DataStore::clear();
  SimulationConfig::outputRegexs.clear();
  SimulationConfig::outputRegexs.push_back("helloWorld");
  res = DataStore::addVariable("helloWorld");
  EXPECT_EQ(res, 0);
  res = DataStore::addVariable("hello1World");
  EXPECT_EQ(res, -1);
  res = DataStore::addVariable("helloWorld1");
  EXPECT_EQ(res, -1);
}

TEST_F(Test_DataStore, regex1) {
  SimulationConfig::outputRegexs.push_back("helloWorld.*");
  res = DataStore::addVariable("helloWorld");
  EXPECT_EQ(res, 0);
  res = DataStore::addVariable("hello1World");
  EXPECT_EQ(res, -1);
  res = DataStore::addVariable("helloWorld1");
  EXPECT_EQ(res, 1);

}

TEST_F(Test_DataStore, regex2) {

  SimulationConfig::outputRegexs.push_back(".*elloWorld.*");
  res = DataStore::addVariable("helloWorld");
  EXPECT_EQ(res, 0);
  res = DataStore::addVariable("hello1World");
  EXPECT_EQ(res, -1);
  res = DataStore::addVariable("helloWorld1");
  EXPECT_EQ(res, 1);
  res = DataStore::addVariable("1helloWorld1");
  EXPECT_EQ(res, 2);

}

TEST_F(Test_DataStore, regex3) {

  SimulationConfig::outputRegexs.push_back(".*ello.*World.*");
  res = DataStore::addVariable("helloWorld");
  EXPECT_EQ(res, 0);
  res = DataStore::addVariable("hello1World");
  EXPECT_EQ(res, 1);
  res = DataStore::addVariable("helloWorld1");
  EXPECT_EQ(res, 2);
  res = DataStore::addVariable("1helloWorld1");
  EXPECT_EQ(res, 3);
}
