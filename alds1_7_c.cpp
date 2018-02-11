#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <vector>
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
    id_(-1),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr) { }

  int id_;
  Node* parent_;
  Node* left_;
  Node* right_;
};

void walkInPreorder(Node *node, vector<int> &vec)
{
  if (node == nullptr)
    return;

  vec.push_back(node->id_);
  walkInPreorder(node->left_, vec);
  walkInPreorder(node->right_, vec);
}

void walkInInorder(Node *node, vector<int> &vec)
{
  if (node == nullptr)
    return;

  walkInInorder(node->left_, vec);
  vec.push_back(node->id_);
  walkInInorder(node->right_, vec);
}

void walkInPostorder(Node *node, vector<int> &vec)
{
  if (node == nullptr)
    return;

  walkInPostorder(node->left_, vec);
  walkInPostorder(node->right_, vec);
  vec.push_back(node->id_);
}

int main()
{
  int numOfNodes;
  cin >> numOfNodes; cin.ignore();

  Node *nodes = new Node[numOfNodes];

  for (int i = 0; i < numOfNodes; i++)
  {
    int id, left, right;
    cin >> id >> left >> right;

    nodes[id].id_ = id;

    if (left > -1)
    {
      nodes[id].left_ = &nodes[left];
      nodes[left].parent_ = &nodes[id];
    }

    if (right > -1)
    {
      nodes[id].right_ = &nodes[right];
      nodes[right].parent_ = &nodes[id];
    }
  }

  assert(&nodes[0] != nullptr);
  Node *root = &nodes[0];

  while (root->parent_ != nullptr)
  {
    root = root->parent_;
  }

  {
    vector<int> vec;
    walkInPreorder(root, vec);

    printf("Preorder\n");

    for (uint32_t i = 0; i < vec.size(); i++)
      printf(" %d",  vec[i]);

    printf("\n");
  }

  {
    vector<int> vec;
    walkInInorder(root, vec);

    printf("Inorder\n");

    for (uint32_t i = 0; i < vec.size(); i++)
    {
      printf(" %d", vec[i]);
    }

    printf("\n");
  }

  {
    vector<int> vec;
    walkInPostorder(root, vec);

    printf("Postorder\n");

    for (uint32_t i = 0; i < vec.size(); i++)
    {
      printf(" %d", vec[i]);
    }

    printf("\n");
  }

  delete[] nodes;

  return 0;
}
