// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include <memory>
#include "tests/Gen.h"

#include "SimulationConfig.h"

#include "Contract.h"
#include "Contract_Node_Priority.h"

#include "gtest/gtest.h"

class Test_Contract_Node_Priority : public ::testing::Test {
 protected:
    Contract_Node_Priority nodePriority;

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Contract_Node_Priority::SetUp() {
  nodePriority.clear();
}

void Test_Contract_Node_Priority::AfterConfiguration() {
}

TEST_F(Test_Contract_Node_Priority, one) {
    Contract c;
    ContractPtr cp = std::make_shared<Contract>(c);
    cp->id = 0;
    cp->buildingID = 0;
    cp->requested = 0.0;
    cp->received = 0.0;
    cp->receivedCost = 0.0;
    cp->supplied = 0.0;
    cp->suppliedCost = 0.0;
    cp->suppliedLeft = 0.0;
    cp->priority = 1.0;
    nodePriority.insert(cp, cp->priority);
    ContractPtr op1;
    op1 = nodePriority.findRightEdge();
    EXPECT_NEAR(op1->received, 0.0, 0.1);
    EXPECT_NEAR(cp->received, 0.0, 0.1);
    op1->received = 10.0;
    EXPECT_NEAR(op1->received, 10.0, 0.1);
    EXPECT_NEAR(cp->received, 10.0, 0.1);
}

TEST_F(Test_Contract_Node_Priority, twoDifferent) {
    Contract c;
    ContractPtr cp = std::make_shared<Contract>(c);
    cp->id = 0;
    cp->buildingID = 0;
    cp->requested = 1000.0;
    cp->received = 100.0;
    cp->priority = 0.0;

    Contract c2;
    ContractPtr cp2 = std::make_shared<Contract>(c2);
    cp2->id = 1;
    cp2->buildingID = 0;
    cp2->requested = 1000.0;
    cp2->received = 100.0;
    cp2->priority = 1.0;

    nodePriority.insert(cp, cp->priority);
    nodePriority.insert(cp2, cp2->priority);

    ContractPtr op1;
    op1 = nodePriority.findRightEdge();
    EXPECT_NEAR(op1->received, 100.0, 0.1);
    EXPECT_EQ(op1->id, 1);

    op1->received = 1000.0;

    ContractPtr op2;
    op2 = nodePriority.findRightEdge();
    EXPECT_NEAR(op2->received, 100.0, 0.1);
    EXPECT_EQ(op2->id, 0);
}


TEST_F(Test_Contract_Node_Priority, twoEqual) {
    Contract c;
    ContractPtr cp = std::make_shared<Contract>(c);
    cp->id = 0;
    cp->buildingID = 0;
    cp->requested = 1000.0;
    cp->received = 100.0;
    cp->priority = 0.0;

    Contract c2;
    ContractPtr cp2 = std::make_shared<Contract>(c2);
    cp2->id = 1;
    cp2->buildingID = 0;
    cp2->requested = 1000.0;
    cp2->received = 100.0;
    cp2->priority = 0.0;

    nodePriority.insert(cp, cp->priority);
    nodePriority.insert(cp2, cp2->priority);

    ContractPtr op1;
    op1 = nodePriority.findRightEdge();
    EXPECT_NEAR(op1->received, 100.0, 0.1);
    EXPECT_EQ(op1->id, 1);

    op1->received = 1000.0;

    ContractPtr op2;
    op2 = nodePriority.findRightEdge();
    EXPECT_NEAR(op2->received, 100.0, 0.1);
    EXPECT_EQ(op2->id, 0);
}
