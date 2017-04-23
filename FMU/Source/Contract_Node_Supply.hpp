// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_NODE_SUPPLY_H_
#define CONTRACT_NODE_SUPPLY_H_

#include "Contract_Node_Tree.hpp"
#include "Contract.hpp"

 /**
  * @brief Tree of contracts sorted by the supplied energy left
  * @details Tree of contracts sorted by the supplied energy left in the contracts
  */

class Contract_Node_Supply : public Contract_Node_Tree<ContractPtr> {
 public:
  Contract_Node_Supply();
  bool compare(const ContractPtr insert) const;
  bool isNodeRemoveable(
        const std::shared_ptr<Contract_Node_Tree<ContractPtr>> & ptr) const;
  void makeLeft();
  void makeRight();
  bool isRemoveable() const;
};

#endif  // CONTRACT_NODE_SUPPLY_H_
