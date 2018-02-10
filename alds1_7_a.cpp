#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <vector>
using namespace std;

#define DEBUG (1)

#if DEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // DEBUG

struct Node {
  Node() :
    id_(-1),
    depth_(-1),
    parentId_(-1) { }

  int           id_;
  int           parentId_;
  int           depth_;
  vector<Node*> leafs_; // TODO: check if memory allocated contiguously?
/*
  uint8_t   depth;
*/
};

Node* getRoot(Node* node, uint32_t numOfNodes)
{
  for (uint32_t i = 0; i < numOfNodes; i++)
  {
    if (node[i].parentId_ == -1)
      return &node[i];
  }

  assert(false);
  return nullptr;
}


void setDepth(Node* node, int depth)
{
  if (node == nullptr)
    return ;

  node->depth_ = depth;
  
  for (uint32_t i = 0; i < node->leafs_.size(); i++)
  {
    setDepth(node->leafs_[i], depth + 1);
  }
}

int main()
{
  uint32_t numOfNodes;
  cin >> numOfNodes;  cin.ignore();

  Node* nodes = new Node[numOfNodes];

  for (uint32_t i = 0; i < numOfNodes; i++)
  {
    int id;
    uint32_t degree;
    cin >> id >> degree;

    nodes[id].id_ = id;

    for (uint32_t j = 0; j < degree; j++)
    {
      uint32_t cid; // the ids will be always sorted in ascending order?
      cin >> cid;
      nodes[id].leafs_.push_back(&nodes[cid]);
      nodes[cid].parentId_ = id;
    }

    cin.ignore();
  }

#if DEBUG
  for (uint32_t i = 0; i < numOfNodes; i++) {
    DPRINTF("%d %lu", nodes[i].id_, nodes[i].leafs_.size());
    for (uint32_t j = 0; j < nodes[i].leafs_.size(); j++) {
      printf(" %d (%p)", nodes[i].leafs_[j]->id_, &nodes[i].leafs_[j]);
    }
    printf("\n");
  }
#endif

  Node* root = getRoot(nodes, numOfNodes); // Assume that tree has the only one root

  uint8_t depth = 0;
  setDepth(root, depth);

  // Print each nodes info in ascending order
  for (uint32_t i = 0; i < numOfNodes; i++) {
    Node* node = &nodes[i];

    string type;
    if (node->depth_ == 0)                type = "root";
    else if (node->leafs_.size() == 0)    type = "leaf";
    else                                  type = "internal node";

    printf("node %d: ", i);
    printf("parent = %d, ", node->parentId_);
    printf("depth = %d, ", node->depth_);
    printf("%s, ", type.c_str());
    printf("[");
    for (uint32_t i = 0; i < node->leafs_.size(); i++) {
      if (i == 0) printf("%d",   node->leafs_[i]->id_);
      else        printf(", %d", node->leafs_[i]->id_);
    }
    printf("]");

    cout << endl;
  }

  delete[] nodes;

  return 0;
}

