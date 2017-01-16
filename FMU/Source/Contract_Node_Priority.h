// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_NODE_PRIORITY_H_
#define CONTRACT_NODE_PRIORITY_H_

#include "Contract_Node_Tree.h"
#include "Contract.h"

class Contract_Node_Priority : public Contract_Node_Tree<ContractPtr> {
 public:
  Contract_Node_Priority();
  bool compare(const ContractPtr insert) const;
  bool isNodeRemoveable(
      const std::shared_ptr<Contract_Node_Tree<ContractPtr>> & ptr) const;
  void makeLeft();
  void makeRight();
  bool isRemoveable() const;
};

#endif  // CONTRACT_NODE_PRIORITY_H_
