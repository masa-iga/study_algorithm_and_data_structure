#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
using namespace std;

//#define DEBUG (1)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else //DEBUG
#define DPRINTF(...)
#endif //DEBUG

template <class T>
struct Node {
  Node* next_ = nullptr;
  Node* prev_ = nullptr;
  T val_;
};

template <class T>
class DoublyLinkedList {
public:
  DoublyLinkedList() :
    head_(nullptr),
    tail_(nullptr) { }
  ~DoublyLinkedList();
  void insert(T);
  void del(T);
  void delFirst();
  void delLast();
  bool empty();
  void printChain();
private:
  Node<T>* head_;
  Node<T>* tail_;
};

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
  Node<T>* node = head_;
  while (node != nullptr) {
    Node<T>* next = node->next_;
    delete(node);
    node = next;
  }
}

template <class T>
void DoublyLinkedList<T>::insert(T x)
{
  if (empty()) {
    head_ = new Node<T>();
    head_->val_ = x;
    tail_ = head_;
    return ;
  }
  Node<T> *node = new Node<T>();
  node->val_ = x;
  node->next_ = head_;
  head_->prev_ = node;
  head_ = node;
}

template <class T>
void DoublyLinkedList<T>::del(T x)
{
  Node<T> *node = head_;
  while (node != nullptr && node->val_ != x) {
    node = node->next_;
  }

  if (node == nullptr) {
#if DEBUG
    cout << "Could not find " << x << endl;
#endif //DEBUG
    return ;
  }

  // be remindful of caring tail_ pointer
  if (node == head_) { delFirst(); return ; }
  if (node == tail_) { delLast(); return ; }

  node->prev_->next_ = node->next_;
  node->next_->prev_ = node->prev_;
  delete(node);
}

template <class T>
void DoublyLinkedList<T>::delFirst()
{
  if (empty()) {
    cout << "Empty!!" << endl;
    return ;
  }

  if (head_ == tail_) {
    delete(head_);
    head_ = nullptr;
    tail_ = nullptr;
    return ;
  }

  head_ = head_->next_;
  delete(head_->prev_);
  head_->prev_ = nullptr;
}

template <class T>
void DoublyLinkedList<T>::delLast()
{
  if (empty()) {
    cout << "Empty!!" << endl;
    return ;
  }

  if (head_ == tail_) {
    delete(head_);
    head_ = nullptr;
    tail_ = nullptr;
    return ;
  }

  tail_ = tail_->prev_;
  delete(tail_->next_);
  tail_->next_ = nullptr;
}

template <class T>
bool DoublyLinkedList<T>::empty() {
  return (head_ == nullptr && tail_ == nullptr);
}

template <class T>
void DoublyLinkedList<T>::printChain()
{
  Node<T> *node = head_;
  while (node != nullptr) {
    if (node != head_)  cout << " ";
    cout << node->val_;
    node = node->next_;
  }
  cout << endl;
}

#define CMD_INSERT        "insert"
#define CMD_DELETE        "delete"
#define CMD_DELETE_FIRST  "deleteFirst"
#define CMD_DELETE_LAST   "deleteLast"

int main() {
  int numOfCommand;
  cin >> numOfCommand; cin.ignore();
  DPRINTF("NUM OF COMMAND : %d\n", numOfCommand);

  DoublyLinkedList<int> list;
  for (int i = 0; i < numOfCommand; i++) {
    int val;
    char command[16];
    scanf("%s %d", command, &val);
    DPRINTF("COMMAND %s\n", command);

    if (!strcmp(command, CMD_INSERT)) {
      list.insert(val);
#if DEBUG
      DPRINTF(""); list.printChain();
#endif //DEBUG
    }
    else if (!strcmp(command, CMD_DELETE))
    {
      list.del(val);
#if DEBUG
      DPRINTF(""); list.printChain();
#endif //DEBUG
    }
    else if (!strcmp(command, CMD_DELETE_FIRST))
    {
      list.delFirst();
#if DEBUG
      DPRINTF(""); list.printChain();
#endif //DEBUG
    }
    else if (!strcmp(command, CMD_DELETE_LAST))
    {
      list.delLast();
#if DEBUG
      DPRINTF(""); list.printChain();
#endif //DEBUG
    }
    else
    {
      cout << "Unknown command : " << command << endl;
    }
  }

  list.printChain();

  return 0;
}
