#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <memory>
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

int g_pCount = 0;

struct Node {
  Node(int key) :
    key_(key),
    left_(nullptr),
    right_(nullptr) {
      g_pCount++;
      DPRINTF("Allocated. (addr %p  key %d  pCount %d)\n", this, this->key_, g_pCount);
    }
  Node() :
    key_(-1),
    left_(nullptr),
    right_(nullptr) {
      Node(-1);
    }

  ~Node() {
    g_pCount--;
    DPRINTF("Deallocated. (addr %p  key %d  pCount %d)\n", this, this->key_, g_pCount);
  }

  int key_;
  Node *left_;
  Node *right_;
};

void insert(Node **root, Node *n)
{

  assert(root != nullptr && n != nullptr && "nullptr catched\n");

  if (*root == nullptr)
  {
    *root = n;
    return;
  }

  Node *parent = nullptr;
  Node *child = *root;

  while (child != nullptr)
  {
    parent = child;
    
    if (n->key_ < child->key_)
      child = child->left_;
    else
      child = child->right_;
  }

  if (n->key_ < parent->key_)
  {
    parent->left_ = n;

    DPRINTF("left - parent->key_ %d  n->key_ %d\n",
        parent->key_, n->key_);
  }
  else
  {
    parent->right_ = n;

    DPRINTF("right - parent->key_ %d  n->key_ %d\n",
        parent->key_, n->key_);
  }

}

void printPreorder(Node *node, bool isRoot = true)
{
  if (node == nullptr)
    return;

  printf(" %d", node->key_);

  printPreorder(node->left_, false);
  printPreorder(node->right_, false);

  if (isRoot)
    cout << endl;
}

void printInorder(Node *node, bool isRoot = true)
{
  if (node == nullptr)
    return;

  printInorder(node->left_, false);
  printf(" %d", node->key_);
  printInorder(node->right_, false);

  if (isRoot)
    cout << endl;
}


bool freeNodePointer(Node *root)
{
  if (root == nullptr)
    return false;

  if (freeNodePointer(root->left_))
    delete(root->left_);

  if (freeNodePointer(root->right_))
    delete(root->right_);

  return true;
}

int main()
{
  int numOfOrder;
  cin >> numOfOrder; cin.ignore();

  Node **root = new (Node*);
  *root = nullptr;

  for (int i = 0; i < numOfOrder; i++)
  {
    string order; cin >> order;

    if (order == "insert")
    {
      int key; cin >> key;
      DPRINTF("%s %d\n", order.c_str(), key);

      Node *n = new Node(key);
      insert(root, n);

    }
    else if (order == "print")
    {
      DPRINTF("%s\n", order.c_str());

      printInorder(*root);
      printPreorder(*root);
    }
    else
    {
      // must not be here
      assert(false && "Unknown order");
    }

    cin.ignore();
  }

  freeNodePointer(*root);
  delete(*root);
  delete(root);

  return 0;
}

