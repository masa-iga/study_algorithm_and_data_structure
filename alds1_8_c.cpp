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

int g_pCount = 0;


struct Node {
  Node(int key) :
    key_(key),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr) {
      g_pCount++;
      DPRINTF("Allocated. (addr %p  key %d  pCount %d)\n", this, this->key_, g_pCount);
    }
  Node(){ Node(-1); }

  ~Node() {
    g_pCount--;
    DPRINTF("Deallocated. (addr %p  key %d  pCount %d)\n", this, this->key_, g_pCount);
  }

  int key_;
  Node *parent_;
  Node *left_;
  Node *right_;
};


void insert(Node (*&root), Node *n)
{

  assert(n != nullptr && "nullptr catched\n");

  if (root == nullptr)
  {
    root = n;
    return;
  }

  Node *parent = nullptr;
  Node *child = root;

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
    n->parent_ = parent;

    DPRINTF("left - parent->key_ %d  n->key_ %d\n",
        parent->key_, n->key_);
  }
  else
  {
    parent->right_ = n;
    n->parent_ = parent;

    DPRINTF("right - parent->key_ %d  n->key_ %d\n",
        parent->key_, n->key_);
  }

}


Node* find_recursive(Node *node, int key)
{
  if (node == nullptr)
    return nullptr;

  if (key == node->key_)
    return node;
  else if (key < node->key_)
    return find_recursive(node->left_, key);
  else
    return find_recursive(node->right_, key);
}


void find(Node *node, int key)
{
  if (find_recursive(node, key) != nullptr)
    cout << "yes" << endl;
  else
    cout << "no" << endl;
}


// Assume the input node has two leaves
Node* findNextNodeInorder(const Node *node)
{

  assert(node != nullptr && node->right_ != nullptr);
  Node *next = node->right_;

  while (next->left_)
    next = next->left_;

  return next;
}

void del(Node (*&node), int key)
{
  if (node == nullptr)
    return ;

  // run time cost: O(h); where h is the depth
  Node *target = find_recursive(node, key);
  if (target == nullptr)
  {
    DPRINTF("not find the node having the key\n");
    return ;
  }

  if (target->left_ == nullptr && target->right_ == nullptr)
  { // having no leaf case

    Node *parent = target->parent_;

    // Invalidate parent's left_/right_ pointer before delete
    if (parent && parent->left_ == target)
      parent->left_ = nullptr;
    else if (parent && parent->right_ == target)
      parent->right_ = nullptr;

    delete(target);
  }
  else if (target->left_ == nullptr || target->right_ == nullptr)
  { // having one leaf case

    Node *child;
    if (target->left_ != nullptr)
      child = target->left_;
    else
      child = target->right_;

    Node *parent = target->parent_;
    if (parent && parent->left_ == target)
      parent->left_ = child;
    else if (parent && parent->right_ == target)
      parent->right_ = child;

    child->parent_ = parent;

    delete(target);
  }
  else
  { // having two leaves case

    Node *nextTarget = findNextNodeInorder(target);
    assert(nextTarget != nullptr);

    target->key_ = nextTarget->key_;
    del(nextTarget, nextTarget->key_);
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


bool freeAllNodes(Node *root)
{
  if (root == nullptr)
    return false;

  if (freeAllNodes(root->left_))
    delete(root->left_);

  if (freeAllNodes(root->right_))
    delete(root->right_);

  return true;
}


int main()
{
  int numOfOrder;
  cin >> numOfOrder; cin.ignore();

  Node *root = nullptr;

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
    else if (order == "find")
    {
      int key; cin >> key;
      DPRINTF("%s %d\n", order.c_str(), key);

      find(root, key);
    }
    else if (order == "delete")
    {
      int key; cin >> key;
      DPRINTF("%s %d\n", order.c_str(), key);

      del(root, key);
    }
    else if (order == "print")
    {
      DPRINTF("%s\n", order.c_str());

      printInorder(root);
      printPreorder(root);
    }
    else
    {
      // must not be here
      assert(false && "Unknown order");
    }

    cin.ignore();
  }

  freeAllNodes(root);
  delete(root);

  return 0;
}

