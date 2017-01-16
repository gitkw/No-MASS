// Copyright 2016 Jacob Chapman

#ifndef CONTRACT_NODE_TREE_H_
#define CONTRACT_NODE_TREE_H_

#include <memory>
#include <iostream>

template <class T>
class Contract_Node_Tree {
 public:
  virtual bool compare(const T insert) const = 0;
  virtual void makeLeft() = 0;
  virtual void makeRight() = 0;
  virtual bool isNodeRemoveable(
            const std::shared_ptr<Contract_Node_Tree<T>> & ptr) const = 0;
  virtual bool isRemoveable() const = 0;

  bool isLeftNull() {
    return pLeft == nullptr;
  }

  bool isRightNull() {
    return pRight == nullptr;
  }

  T findRightEdge() {
    std::cout << " val: " << value << std::endl;
    T ret = nodeObject;
    if (pRight) {
      std::cout << "right" << std::endl;
      if (isNodeRemoveable(pRight)) {
        pRight  = nullptr;
      } else {
        ret = pRight->findRightEdge();
      }
    }
    if (isAssigned && isRemoveable()) {
      std::cout << "cent" << std::endl;
      isAssigned = false;
      nodeObject = NULL;
    }
    if (pLeft && isAssigned == false) {
      std::cout << "left" << std::endl;
      if (isNodeRemoveable(pLeft)) {
        pLeft = nullptr;
      } else {
        ret = pLeft->findRightEdge();
      }
    }
    if (ret == nullptr && (pRight != nullptr || pLeft != nullptr)) {
      ret = findRightEdge();
    }
    return ret;
  }

  T findLeftEdge() {
    if (pLeft) {
      if (isNodeRemoveable(pLeft)) {
        pLeft  = nullptr;
      } else {
        return pLeft->findLeftEdge();
      }
    }
    if (isAssigned && isRemoveable()) {
      isAssigned = false;
      nodeObject = NULL;
    }
    if (pRight && isAssigned == false) {
      if (isNodeRemoveable(pRight)) {
        pRight  = nullptr;
      } else {
        return pRight->findLeftEdge();
      }
    }
    return nodeObject;
  }

  T popLeftEdge() {
    T ret;
    if (pLeft) {
        return pLeft->popLeftEdge();
    } else if (isAssigned) {
      isAssigned = false;
      ret = nodeObject;
      nodeObject = NULL;
    } else if (pRight) {
      return pRight->popLeftEdge();
    }
    return ret;
  }

  void insert(const T & insert, double value) {
    std::cout << "insert";
    if (nodeObject == NULL) {
      nodeObject = insert;
      this->value = value;
      isAssigned = true;
    } else {
      if (value < this->value) {
        if (!pLeft) {
          makeLeft();
        }
        pLeft->insert(insert, value);
      } else {
        if (!pRight) {
          makeRight();
        }
        pRight->insert(insert, value);
      }
    }
  }

  const T getNodeObject() const {
    return nodeObject;
  }

  void clear() {
    pLeft.reset();
    pRight.reset();
    isAssigned = false;
    nodeObject = NULL;
  }

 protected:
  T nodeObject;
  Contract_Node_Tree() {}
  std::shared_ptr<Contract_Node_Tree<T>> pLeft;
  std::shared_ptr<Contract_Node_Tree<T>> pRight;

 private:
  bool isAssigned = false;
  double value;
};

#endif  // CONTRACT_NODE_TREE_H_
