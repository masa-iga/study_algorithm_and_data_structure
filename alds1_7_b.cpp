#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
using namespace std;

#define DEBUG (0)

#if DEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // DEBUG

struct Node
{
  Node() :
    id_ (-1),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr) { }

  int id_;
  Node* parent_;
  Node* left_;   // child at the left side
  Node* right_;  // child at the right side

  int getParentId();
  int getSiblingId();
  uint8_t getDegree();
  uint8_t getDepth(uint8_t depth);
  uint8_t getHeight(uint8_t height);
  string getNodeType();
};


int Node::getParentId()
{
  if (parent_ == nullptr)
    return -1;

  return parent_->id_;
}

int Node::getSiblingId()
{
  if (parent_ == nullptr)
    return -1;

  Node* sibling;
  if (parent_->left_ != this)
    sibling = parent_->left_;
  else
    sibling = parent_->right_;

  if (sibling == nullptr)
    return -1;
  else
    return sibling->id_;
}

uint8_t Node::getDegree()
{
  int degree = 0;
  if (left_ != nullptr)
    degree++;

  if (right_ != nullptr)
    degree++;

  return degree;
}

uint8_t Node::getDepth(uint8_t depth = 0)
{
  if (parent_ == nullptr)
    return depth;

  return parent_->getDepth(depth + 1);
}

string Node::getNodeType()
{
  if (parent_ == nullptr)
    return "root";

  if (left_ == nullptr && right_ == nullptr)
    return "leaf";

  return "internal node";
}

uint8_t Node::getHeight(uint8_t height = 0)
{
  //DPRINTF("node %d: left %p  right %p\n", id_, left_, right_);

  uint8_t leftHeight, rightHeight;

  if (left_ == nullptr)     leftHeight = height;
  else                      leftHeight = left_->getHeight(height + 1);

  if (right_ == nullptr)    rightHeight = height;
  else                      rightHeight = right_->getHeight(height + 1);

  return max(leftHeight, rightHeight);
}

int main()
{
  int numOfNodes;
  cin >> numOfNodes; cin.ignore();

  Node* nodes = new Node[numOfNodes];

  for (int i = 0; i < numOfNodes; i++)
  {
    int parent, left, right;
    cin >> parent >> left >> right;
    assert(left < numOfNodes && right < numOfNodes);

    nodes[parent].id_ = parent;

    if (left > -1)
    {
      nodes[parent].left_ = &nodes[left];
      nodes[left].parent_ = &nodes[parent];
    }

    if (right > -1)
    {
      nodes[parent].right_ = &nodes[right];
      nodes[right].parent_ = &nodes[parent];
    }

  }

#if DEBUG
  for (int i = 0; i < numOfNodes; i++) {
    int id;
    DPRINTF("node %d (%p): ", i, &nodes[i]);

    if (nodes[i].parent_ == nullptr)    id = -1;
    else                                id = nodes[i].getParentId();
    printf("parent = %d, ", id);

    if (nodes[i].left_ == nullptr)      id = -1;
    else                                id = nodes[i].left_->id_;
    printf("left = %d, ", id);

    if (nodes[i].right_ == nullptr)     id = -1;
    else                                id = nodes[i].right_->id_;
    printf("right = %d", id);

    cout << endl;
  }
#endif // DEBUG

  /*
  Output : parent, sibling, degree, depth, height [root, internal node, leaf]
  */

  for (int i = 0; i < numOfNodes; i++)
  {
    printf("node %d: ", i);
    printf("parent = %d, ", nodes[i].getParentId());
    printf("sibling = %d, ", nodes[i].getSiblingId());
    printf("degree = %d, ", nodes[i].getDegree());
    printf("depth = %d, ", nodes[i].getDepth());
    printf("height = %d, ", nodes[i].getHeight());
    printf("%s", nodes[i].getNodeType().c_str());

    cout << endl;
  }

  return 0;
}

