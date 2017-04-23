// Copyright 2016 Jacob Chapman
#include <memory>
#include "Contract_Node_Priority.hpp"


Contract_Node_Priority::Contract_Node_Priority() {}

bool Contract_Node_Priority::compare(const ContractPtr insert) const {
  return insert->priority < nodeObject->priority;
}

bool Contract_Node_Priority::isNodeRemoveable(
          const std::shared_ptr<Contract_Node_Tree<ContractPtr>> & ptr) const {

    bool retValue = ptr->isLeftNull() && ptr->isRightNull();
    if (ptr->getNodeObject() != nullptr) {
        retValue = ptr->isRemoveable() && retValue;
    }
    return retValue;
}

void Contract_Node_Priority::makeLeft() {
  pLeft = std::make_shared<Contract_Node_Priority>();
}

void Contract_Node_Priority::makeRight() {
  pRight = std::make_shared<Contract_Node_Priority>();
}

bool Contract_Node_Priority::isRemoveable() const {
  return getNodeObject()->received >= getNodeObject()->requested;
}
