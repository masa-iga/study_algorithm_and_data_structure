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

const string CMD_INSERT = "insert";
const string CMD_EXTRACT = "extract";
const string CMD_END = "end";


inline void swap(int &a, int &b) {
  int c = b; b = a; a = c;
}


struct PriorityQueue {
  PriorityQueue() :
    num_(0) {
      pArray_ = new int[kMaxIndex];
    }

  ~PriorityQueue() {
    if (pArray_)
      delete [] pArray_;
  }

  const int kMaxIndex = (64 - 1) * 1024 * 1024;

  void insert(int val);  // run time: O(logN)
  int extractMax(); // run time: O(logN)

  bool maxHeapify(int idx);
  void shiftUp(int idx);
  int getTailIndex() { return num_ - 1; }
  int getParentIndex(int idx);

  void print();

  int num_;
  int *pArray_;
};


int PriorityQueue::getParentIndex(int idx) {
  if (idx == 0)
    return -1;

  return (idx - 1) / 2;
}


void PriorityQueue::insert(int val) {
  DPRINTF("val %d  num_ %d\n", val, num_);

  // add into the tail
  pArray_[num_] = val;
  num_++;

  bool stillContinue = true;
  int idx = getTailIndex();
  int parentIdx = getParentIndex(idx);

  while (parentIdx >= 0 && stillContinue) {
    if (pArray_[idx] > pArray_[parentIdx]) {
      swap(pArray_[idx], pArray_[parentIdx]);
      stillContinue = true;
    } else {
      stillContinue = false;
    }

    idx = parentIdx;
    parentIdx = getParentIndex(idx);
  }

}


bool PriorityQueue::maxHeapify(int idx) {

  DPRINTF("idx %d  num_ %d\n", idx, num_);
  assert(0 <= idx && idx < num_);

  int largest;
  int leftIdx = 2 * idx + 1;
  int rightIdx = 2 * idx + 2;

  if (leftIdx < num_ && pArray_[leftIdx] > pArray_[idx])
    largest = leftIdx;
  else
    largest = idx;

  if (rightIdx < num_ && pArray_[rightIdx] > pArray_[largest])
    largest = rightIdx;

  DPRINTF("largest %d\n", largest);

  if (largest != idx) {
    swap(pArray_[idx], pArray_[largest]);
    maxHeapify(largest);
    return true;
  } else {
    return false;
  }
}


int PriorityQueue::extractMax() {

  assert(num_ > 0);

  const int headIndex = 0;
  const int greatest = pArray_[headIndex];

  pArray_[headIndex] = pArray_[getTailIndex()];
  num_--;

  maxHeapify(headIndex);

  return greatest;
}


void PriorityQueue::shiftUp(int idx) {
  if (idx < 1)
    return ;

  int parrentIdx = (idx - 1) / 2;
  shiftUp(parrentIdx);
  pArray_[parrentIdx] = pArray_[idx];
}


void PriorityQueue::print() {
  DPRINTF("");
  for (int i = 0; i < num_; i++)
    printf(" %d", pArray_[i]);

  cout << endl;
}


int main() {

  PriorityQueue queue;
  bool isEnd = false;

  do {
    string command;
    cin >> command;

    if (command == CMD_INSERT)
    {
      int val; cin >> val;
      DPRINTF("insert %d\n", val);

      queue.insert(val);

#if DEBUG
      queue.print();
#endif // DEBUG

    }
    else if (command == CMD_EXTRACT)
    {
      DPRINTF("extract\n");
      cout << queue.extractMax() << endl;;

#if DEBUG
      queue.print();
#endif // DEBUG

    }
    else if (command == CMD_END)
    {
      DPRINTF("end\n");
      isEnd = true;
    }
    else
    {
      printf("Unkown command, %s\n", command.c_str());
      assert(false);
    }

    cin.ignore();

  } while (!isEnd);

  return 0;
}

