// Copyright 2016 Jacob Chapman
#include <memory>
#include "Contract_Priority_Node.h"


Contract_Priority_Node::Contract_Priority_Node() {}

bool Contract_Priority_Node::compare(const ContractPtr insert) const {
  return insert->priority < nodeObject->priority;
}

bool Contract_Priority_Node::isNodeRemoveable(
                    const std::shared_ptr<Tree_Node<ContractPtr>> & ptr) const {
    bool retValue = ptr->isLeftNull() && ptr->isRightNull();
    
    if (ptr->getNodeObject() != nullptr){
        retValue = ptr->getNodeObject()->received >= ptr->getNodeObject()->requested && ptr->isLeftNull() && ptr->isRightNull();
    }
    return retValue;
}

void Contract_Priority_Node::makeLeft() {
  pLeft = std::make_shared<Contract_Priority_Node>();
}

void Contract_Priority_Node::makeRight() {
  pRight = std::make_shared<Contract_Priority_Node>();
}

bool Contract_Priority_Node::isRemoveable() const {
  return getNodeObject()->received >= getNodeObject()->requested;
}
