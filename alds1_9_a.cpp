#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <stack>
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
  Node(int id, int key) :
    id_(id),
    key_(key) {
      DPRINTF("Allocated. (id %d  key %d) @ %p\n", id_, key_, this);

      pParent_ = nullptr;
      pLeft_ = nullptr;
      pRight_ = nullptr;
    }

  Node() :
    id_(-1),
    key_(-1) {
      DPRINTF("Allocated. (id %d  key %d) @ %p\n", id_, key_, this);

      pParent_ = nullptr;
      pLeft_ = nullptr;
      pRight_ = nullptr;
    }

  ~Node() {
      DPRINTF("Deleted. (id %d  key %d) @ %p\n", id_, key_, this);
  }

  void set(int id, int key);

  int id_;
  int key_;
  Node *pParent_;
  Node *pLeft_;
  Node *pRight_;
};


void Node::set(int id, int key)
{
  DPRINTF("%p  id %d   key %d\n", this, id, key);

  id_ = id;
  key_ = key;
}


struct CompleteBinTree {
  CompleteBinTree() :
    pRoot_(nullptr),
    num_(0) { }

  void add(Node *pNode);
  void printAllNodes();

  Node *pRoot_;
  int num_;
};


void CompleteBinTree::add(Node *pNew)
{
  if (pRoot_ == nullptr)
  {
    pRoot_ = pNew;
    num_++;
    return ;
  }

  stack<int> st;
  int n = num_ + 1;

  while (n > 1)
  {
    int r = n % 2;
    n = n / 2;

    st.push(r);
  }


  Node *pNode = pRoot_;

  while (st.size() > 1)
  {
    if (st.top() == 0)
      pNode = pNode->pLeft_;
    else
      pNode = pNode->pRight_;

    st.pop();
  }

  if (st.top() == 0)
    pNode->pLeft_= pNew;
  else
    pNode->pRight_ = pNew;

  pNew->pParent_ = pNode;
  num_++;

}


void printAllNodes(const Node *pRoot, int num)
{
  for (int i = 0; i < num; i++)
  {
    const Node *pNode = pRoot + i;

    printf("node %d: ", pNode->id_);
    printf("key = %d, ", pNode->key_);
    if (pNode->pParent_)   printf("parent key = %d, ", pNode->pParent_->key_);
    if (pNode->pLeft_)     printf("left key = %d, ", pNode->pLeft_->key_);
    if (pNode->pRight_)    printf("right key = %d, ", pNode->pRight_->key_);

    cout << endl;
  }
}


void freeNodes(Node *pNode)
{
  if (pNode == nullptr)
    return ;
}

int main()
{
  CompleteBinTree *binaryTree = new CompleteBinTree();

  int numOfNode;
  cin >> numOfNode; cin.ignore();

  DPRINTF("numOfNode %d\n", numOfNode);

  // Allocate numOfNode size of Node memory contiguously at the same time;
  // This will make us easily to access each of node,
  // and we can expect better performance because there will be more chances that cache hits
  Node *pNodes = new Node[numOfNode];
  DPRINTF("pNodes %p\n", pNodes);

  for (int i = 0; i < numOfNode; i++)
  {
    int key;
    cin >> key; cin.ignore();

    DPRINTF("node %d  key %d\n", i, key);

    const int id = i + 1;
    pNodes[i].set(id, key);

    binaryTree->add(&pNodes[i]);
  }

  printAllNodes(pNodes, numOfNode);

  delete [] pNodes;

  return 0;
}

