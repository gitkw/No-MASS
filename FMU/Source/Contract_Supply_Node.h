// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_SUPPLY_NODE_H_
#define CONTRACT_SUPPLY_NODE_H_

#include "Tree_Node.h"
#include "Contract.h"

class Contract_Supply_Node : public Tree_Node<ContractPtr> {
 public:
  Contract_Supply_Node();
  bool compare(const ContractPtr insert) const;
  bool isNodeRemoveable(const std::shared_ptr<Tree_Node<ContractPtr>> & ptr)
                                                                      const;
  void makeLeft();
  void makeRight();
  bool isRemoveable() const;
};

#endif  // CONTRACT_SUPPLY_NODE_H_
