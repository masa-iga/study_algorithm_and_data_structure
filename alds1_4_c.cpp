#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

#define DEBUG (0)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

static const int table_size = 1 * 1000 * 1000;
static const string CMD_INSERT = "insert";
static const string CMD_FIND = "find";

struct Node {
  string str;
  Node* next = nullptr;
};

struct NodeChain {
  Node* node = nullptr;
  NodeChain* next = nullptr;
};

class HashTable {
public:
  HashTable();
  ~HashTable();
  void insert(string key);
  bool find(string key);
private:
  Node* nodes[table_size];
  uint32_t stringToHash(char *word, uint32_t hashTableSize);
};

HashTable::HashTable()
{
  for (int i = 0; i < table_size; i++) {
    nodes[i] = nullptr;
  }
}

HashTable::~HashTable()
{
  for (int i = 0; i < table_size; i++) {
    Node *node = nodes[i];
    while (node != nullptr) {
      Node *next = node->next;
      DPRINTF("Deleted a node %p (IDX %d)\n", node, i);
      delete(node);
      node = next;
    }
  }
}

void HashTable::insert(string key)
{
  uint32_t idx = stringToHash(const_cast<char*>(key.c_str()), table_size);
  Node **node = &nodes[idx];
  while (*node != nullptr) {
    *node = (*node)->next;
  }
  *node = new Node();
  DPRINTF("Allocated a new node %p (IDX %d)\n", *node, idx);
  (*node)->str = key;
}

bool HashTable::find(string key)
{
  uint32_t idx = stringToHash(const_cast<char*>(key.c_str()), table_size);
  Node *node = nodes[idx];
  DPRINTF("IDX %d  NODE %p\n", idx, node);

  if (node == nullptr)
    return false;

  while (node != nullptr && node->str != key) {
    node = node->next;
  }

  if (node == nullptr)
    return false;
  else
    return true;
}

uint32_t HashTable::stringToHash(char *word, uint32_t hashTableSize)
{
   uint32_t counter, hashAddress =0;
   for (counter =0; word[counter]!='\0'; counter++)
   {
     hashAddress = hashAddress*word[counter] + word[counter] + counter;
   }
   return (hashAddress%hashTableSize);
}

int main() {
  int numOfOrder;
  cin >> numOfOrder; cin.ignore();

  HashTable hashTable;
  string order, str;
  for (int i = 0; i < numOfOrder; i++) {
    cin >> order >> str;
    DPRINTF("%s %s\n", order.c_str(), str.c_str());

    if (order == CMD_INSERT)
    {
      hashTable.insert(str);
    }
    else if (order == CMD_FIND)
    {
      if(hashTable.find(str))
        cout << "yes" << endl;
      else
        cout << "no" << endl;
    }
    else
    {
      DPRINTF("Unknown order %s\n", order.c_str());
      assert(1);
    }
  }
  return 0;
}
