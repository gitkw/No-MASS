// Copyright 2016 Jacob Chapman
#include <memory>
#include "Contract_Node_Supply.h"

Contract_Node_Supply::Contract_Node_Supply() {}

bool Contract_Node_Supply::compare(const ContractPtr insert) const {
  return insert->suppliedCost < nodeObject->suppliedCost;
}

bool Contract_Node_Supply::isNodeRemoveable(
          const std::shared_ptr<Contract_Node_Tree<ContractPtr>> & ptr) const {
  return ptr->getNodeObject()->suppliedLeft <= 0;
}

void Contract_Node_Supply::makeLeft() {
  pLeft = std::make_shared<Contract_Node_Supply>();
}

void Contract_Node_Supply::makeRight() {
  pRight = std::make_shared<Contract_Node_Supply>();
}

bool Contract_Node_Supply::isRemoveable() const {
  return getNodeObject()->suppliedLeft <= 0;
}
