#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
using namespace std;

//#define DEBUG (1)

#ifdef DEBUG
#define DPRINT(...)  printf("D(L%d)%s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

struct Card {
  Card() { };
  Card(char suit, int value) :
    suit_(suit),
    value_(value) { };

  char suit_;
  int value_;
  void print(){ cout << suit_ << value_; }
};

class Solution {
public:
  Solution();
  ~Solution();
  void pre();
  void run();
  void post();
private:
  int dataSize;
  Card *inputCards;
  Card* copyCards(const Card* pCard, const int size);
  void bubbleSort(Card* pCard, const int size);
  void selectionSort(Card* pCard, const int size);
  void swap(Card &c0, Card &c1);
  void printArray(const Card* pCard, const int size);
  bool isStable(const Card* p0, const Card* p1, const int size);
};

Solution::Solution() {
  cin >> dataSize; cin.ignore();
  assert(dataSize > 0);

  inputCards = new Card[dataSize];
  for (int i=0; i < dataSize; i++)
    cin >> inputCards[i].suit_ >> inputCards[i].value_;

#ifdef DEBUG
  cout << "INPUT: "; printArray(inputCards, dataSize);
#endif
}

Solution::~Solution() {
  if (inputCards)   delete [] inputCards;
}

void Solution::pre() { }

void Solution::run() {
  Card* cards0 = copyCards(inputCards, dataSize);
  bubbleSort(cards0, dataSize);
  printArray(cards0, dataSize);
  isStable(inputCards, cards0, dataSize);
  delete [] cards0;

  Card* cards1 = copyCards(inputCards, dataSize);
  selectionSort(cards1, dataSize);
  printArray(cards1, dataSize);
  isStable(inputCards, cards1, dataSize);
  delete [] cards1;
}

void Solution::post() { }

Card* Solution::copyCards(const Card* pCard, const int size) {
  Card* newCards = new Card[size];
  for (int i=0; i < size; i++)
    newCards[i] = pCard[i];
  return newCards;
}

void Solution::bubbleSort(Card* pCard, const int size) {
  if (pCard == nullptr || size <= 1) return;

  for (int i = 0; i < size; i++) {
    for (int j = size-1; j > i; j--) {
      if (pCard[j].value_ < pCard[j-1].value_)
        swap(pCard[j], pCard[j-1]);
    }
  }
}

void Solution::selectionSort(Card* pCard, const int size) {
  for (int i = 0; i < size-1; i++) {
    int min = i;
    for (int j = i+1; j < size; j++) {
      if (pCard[j].value_ < pCard[min].value_)
        min = j;
    }
    if (min != i)
      swap(pCard[min], pCard[i]);
  }
}

void Solution::printArray(const Card* pCard, const int size) {
  for (int i=0; i < size; i++) {
    if (i > 0)  cout << " ";
    cout << pCard[i].suit_ << pCard[i].value_;
  }
  cout << endl;
}

void Solution::swap(Card &c0, Card &c1) {
  Card t = c0;
  c0 = c1;
  c1 = t;
}

/* Run time: O(n)
 * Memory : O(2n)
 */
bool Solution::isStable(const Card* p0, const Card* p1, const int size) {
  // Card value ranges from 1 to 9
  const int start_val = 1;
  const int end_val = 9;
  vector< vector<char> > vv0(9), vv1(9);

  for (int i = 0; i < size; i++) {
    vv0[p0[i].value_ - start_val].push_back(p0[i].suit_);
    vv1[p1[i].value_ - start_val].push_back(p1[i].suit_);
  }

  // Assume each card sorted out already; just check its stability here
  for (int i = 0; i < end_val; i++) {
    if (vv0[i].size()) {
      for (int j = 0; j < vv0[i].size(); j++) {
        if (vv0[i][j] != vv1[i][j]) {
          cout << "Not stable" << endl;
          return false;
        }
      }
    }
  }
  cout << "Stable" << endl;
  return true;
}


int main() {
  Solution solution;
  solution.pre();
  solution.run();
  solution.post();
  return 0;
}

// TODO: compare with the answer code
