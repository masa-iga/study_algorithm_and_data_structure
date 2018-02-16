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


struct Node {
  Node() :
    id_(-1),
    left_(nullptr),
    right_(nullptr) { }

  int id_;
  Node* left_;
  Node* right_;
};


int findIndexOf(int val, int start, int end, const int array[])
{
  for (int i = start; i <= end; i++)
  {
    if (array[i] == val)
      return i;
  }

  // The val must be found between start to end index
  assert(false);
}

Node* reconstructTree(Node *node, int start, int end, const int preorder[], const int inorder[])
{
  DPRINTF("start %d  end %d\n", start, end);

#if DEBUG
  if (true) {
    for (int i = start; i <= end; i++)
    {
      if (i == start) DPRINTF("preorder: %d", preorder[i]);
      else            printf(" %d", preorder[i]);
    }
    cout << endl;

    for (int i = start; i <= end; i++)
    {
      if (i == start) DPRINTF("inorder:  %d", inorder[i]);
      else            printf(" %d", inorder[i]);
    }
    cout << endl;
  }
#endif

  // Index conversion needed because node index ranges from 0 to N-1, while given index from input ranges from 1 to N
  const int kOffset = 1;
  const int rootNodeId = preorder[start];
  const int nodeIndex = rootNodeId - kOffset;

  // leaf
  if (start == end)
    return &node[nodeIndex];

  // nullptr
  if (start > end)
    return nullptr;

  // Determine which node is the root in this sub-tree
  const int rootIndexOfInorder = findIndexOf(rootNodeId, start, end, inorder);

  DPRINTF("rootNodeId %d  rootIndexOfInorder %d\n", rootNodeId, rootIndexOfInorder);

  // Similarly, determine the root nodes of both the left and right sub-tree.
  // Note that pre-order array has to be alligned before dive into recursively left side sub-tree.
  int newPreorder[rootIndexOfInorder];

  for (int i = start; i < rootIndexOfInorder; i++)
    newPreorder[i] = preorder[i+1];

#if DEBUG
  if (false) {
    for (int i = start; i < rootIndexOfInorder; i++)
    {
      if (i == start) DPRINTF("newPreorder: %d", newPreorder[i]);
      else            printf(" %d", newPreorder[i]);
    }
    cout << endl;
  }
#endif

  node[nodeIndex].left_ = reconstructTree(node, start, rootIndexOfInorder-1, newPreorder, inorder);
  node[nodeIndex].right_ = reconstructTree(node, rootIndexOfInorder+1, end, preorder, inorder);

  if (true) {
    int leftIndex = -1, rightIndex = -1;

    if (node[nodeIndex].left_)
      leftIndex = node[nodeIndex].left_->id_;

    if (node[nodeIndex].right_)
      rightIndex = node[nodeIndex].right_->id_;

    DPRINTF("node %d / left %d / right %d\n", node[nodeIndex].id_, leftIndex, rightIndex);
  }

  return &node[nodeIndex];
}


void traversePostorder(const Node *node)
{
  static bool isCalledAlready = false;

  if (node == nullptr)
    return ;

  if (node->left_ != nullptr)
    traversePostorder(node->left_);

  if (node->right_ != nullptr)
    traversePostorder(node->right_);

  if (isCalledAlready)
    printf(" ");
  else
    isCalledAlready = true;

  printf("%d", node->id_);

}


int main()
{
  int numOfNode;
  cin >> numOfNode; cin.ignore();
  DPRINTF("numOfNode %d\n", numOfNode);

  int arrayOfPreorder[numOfNode];
  int arrayOfInorder[numOfNode];

  for (int i = 0; i < numOfNode; i++)
  {
    cin >> arrayOfPreorder[i];
    assert(arrayOfPreorder[i] <= numOfNode);
  }

  for (int i = 0; i < numOfNode; i++)
  {
    cin >> arrayOfInorder[i];
    assert(arrayOfPreorder[i] <= numOfNode);
  }

#if DEBUG
  for (int i = 0; i < numOfNode; i++)
  {
    if (i == 0) DPRINTF("%d", arrayOfPreorder[i]);
    else        printf(" %d", arrayOfPreorder[i]);
  }
  cout << endl;

  for (int i = 0; i < numOfNode; i++)
  {
    if (i == 0) DPRINTF("%d", arrayOfInorder[i]);
    else        printf(" %d", arrayOfInorder[i]);
  }
  cout << endl;
#endif

  Node *nodes = new Node[numOfNode];

  // Assue input index ranges from 1 to N, while node index ranges from 0 to N-1
  for (int i = 0; i < numOfNode; i++)
    nodes[i].id_ = i + 1;

  int start = 0;
  int end = numOfNode - 1;
  Node *root = reconstructTree(nodes, start, end, arrayOfPreorder, arrayOfInorder);

  traversePostorder(root);
  cout << endl;

  delete[] nodes;

  return 0;
}

