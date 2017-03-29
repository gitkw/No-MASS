// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "tests/Gen.h"

#include "SimulationConfig.h"
#include "Utility.h"
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


TEST_F(Test_DataStore, many) {

  SimulationConfig::outputRegexs.push_back(".*ello.*World.*");
  for (int i = 0; i < 10000; i++) {
    int res = -1;
    res = DataStore::addVariable("1helloWorld" + std::to_string(i));
    double val = Utility::randomDouble(0,100);
    DataStore::addValue(res, val);
    double ret = DataStore::getValue(res);
    EXPECT_NEAR(val, ret, 0.001);
  }
  DataStore::clear();
}


TEST_F(Test_DataStore, save) {
  SimulationConfig::info.save = true;
  SimulationConfig::outputRegexs.clear();
  SimulationConfig::outputRegexs.push_back(".*ello.*World.*");
  int res = -1;
  std::vector<std::string> names;
  std::vector<int> ids;
  std::vector<std::vector<double>> values;
  std::string name;
  name = "1helloWorlderrrrrr";
  res = DataStore::addVariable(name);
  names.push_back(name);
  ids.push_back(res);
  values.push_back(std::vector<double>());
    int z = 10;
    int y = 10;

  for (int i = 0; i < z; i++) {
    name = "1helloWorld" + std::to_string(i);
    res = DataStore::addVariable(name);
    names.push_back(name);
    ids.push_back(res);
    std::vector<double> vs;
    for(int j = 0; j < y; j++) {
      double val = Utility::randomDouble(0,100);
      DataStore::addValue(res, val);
      vs.push_back(val);
      double ret = DataStore::getValue(res);
      EXPECT_NEAR(val, ret, 0.001);
    }
    values.push_back(vs);
  }


  name = "1helloWorlderrrrrr2";
  res = DataStore::addVariable(name);
  names.push_back(name);
  ids.push_back(res);
  values.push_back(std::vector<double>());

  for (int i = 0; i < z; i++) {
      name = "2helloWorld" + std::to_string(i);
      res = DataStore::addVariable(name);
      names.push_back(name);
      ids.push_back(res);
      std::vector<double> vs;
      for(int j = 0; j < y; j++) {
          double val = Utility::randomDouble(1,100);
          DataStore::addValue(res, val);
          vs.push_back(val);
          double ret = DataStore::getValue(res);
          EXPECT_NEAR(val, ret, 0.001);
      }
      values.push_back(vs);
  }


  DataStore::print();
  DataStore::clear();
  std::vector<std::string> head = Utility::csvToTableHead("NoMASS.out");
  Utility::uTable<double> t = Utility::csvToTable<double>("NoMASS.out", true);
    std::vector<std::vector<double>> v;
  for(int i = 0; i < t[0].size(); i++){
      
    std::vector<double> vs;
    for(int j = 0; j < t.size(); j++){
        vs.push_back(t[j][i]);
    }
    v.push_back(vs);
  }

  for(int i = 1; i < head.size(); i++){
    std::string h = head[i];
    std::vector<double> vs = v[i];
    int x = 0;
    for (;x<names.size();x++){
      name = names[x];
      if(name == h){
        break;
      }
    }
    std::vector<double> vs2 = values[x];
    if (vs[0] > 0){
      EXPECT_EQ(vs.size(), vs2.size());
      for(int j = 0; j < vs.size(); j++){
          EXPECT_NEAR(vs[j], vs2[j], 0.01);
      }
    }else {
        EXPECT_EQ(0, vs2.size());
    }
  }
}
