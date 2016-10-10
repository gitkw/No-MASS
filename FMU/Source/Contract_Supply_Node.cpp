// Copyright 2016 Jacob Chapman
#include <memory>
#include "Contract_Supply_Node.h"

Contract_Supply_Node::Contract_Supply_Node() {}

bool Contract_Supply_Node::compare(const ContractPtr insert) const {
  return insert->suppliedCost < nodeObject->suppliedCost;
}

bool Contract_Supply_Node::isNodeRemoveable(
                    const std::shared_ptr<Tree_Node<ContractPtr>> & ptr) const {
  return ptr->getNodeObject()->suppliedLeft <= 0;
}

void Contract_Supply_Node::makeLeft() {
  pLeft = std::make_shared<Contract_Supply_Node>();
}

void Contract_Supply_Node::makeRight() {
  pRight = std::make_shared<Contract_Supply_Node>();
}

bool Contract_Supply_Node::isRemoveable() const {
  return getNodeObject()->suppliedLeft <= 0;
}
