#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <algorithm>
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

class Card
{
public:
  enum Suit {
    Club = 0,
    Diamond,
    Heart,
    Spade,
  };

  Card() { }
  Card(int v, char s) :
    suit_(getSuitFromChar(s)),
    faceValue_(v) { }

  bool operator==(const Card &others) const {
    return ((*this).suit_ == others.suit_) & ((*this).faceValue_ == others.faceValue_);
  }

  bool operator!=(const Card &others) const {
    return !(*this == others);
  }

  void set(int v, char s);
  char getCharFromSuit();
  inline int getValue() { return faceValue_; };

private:
  Suit suit_;
  int faceValue_;

  Suit getSuitFromChar(char s);
};

void Card::set(int v, char s)
{
  suit_ = getSuitFromChar(s);
  faceValue_ = v;
}

char Card::getCharFromSuit()
{
  switch (suit_)
  {
    case Club    : return 'C';
    case Diamond : return 'D';
    case Heart   : return 'H';
    case Spade   : return 'S';
    default: break;
  }

  return ' ';
}

Card::Suit Card::getSuitFromChar(char suit)
{
  switch (suit) {
    case 'C' : return Club;
    case 'D' : return Diamond;
    case 'H' : return Heart;
    case 'S' : return Spade;
    default: break;
  }

  assert(1); // do not reach here
  return Card::Suit::Club;
}

void swap(Card* pCard, int i0, int i1)
{
  Card temp = pCard[i1];
  pCard[i1] = pCard[i0];
  pCard[i0] = temp;
}

int partition(Card* pCard, int start, int end)
{
  int pivot = pCard[end].getValue();
  int pos = start - 1;

  for (int i = start; i < end; i++)
  {
    if (pCard[i].getValue() <= pivot)
    {
      pos++;
      swap(pCard, pos, i);
    }
  }

  swap(pCard, pos + 1, end);

  return pos + 1;
}

void quickSort(Card* pCard, int start, int end)
{
  if (start >= end)
    return;

#if DEBUG
  DPRINTF("Sort Befor -------------\n");
  for (int i = start; i <= end; i++)
    DPRINTF("%d: %c %d\n", i, pCard[i].getCharFromSuit(), pCard[i].getValue());
#endif

  int pos = partition(pCard, start, end);

#if DEBUG
  DPRINTF("Sort After -------------\n");
  for (int i = start; i <= end; i++)
    DPRINTF("%d: %c %d\n", i, pCard[i].getCharFromSuit(), pCard[i].getValue());
#endif

  quickSort(pCard, start, pos - 1);
  quickSort(pCard, pos + 1, end);
}

void bubbleSort(Card* pCard, int size)
{
  for (int i = 0; i < size; i++)
  {
    for (int j = size - 1; j > i; j--)
    {
      if (pCard[j-1].getValue() > pCard[j].getValue())
        swap(pCard, j-1, j);
    }
  }
}

bool comp_card(Card card0, Card card1)
{
  return card0.getValue() < card1.getValue();
}

int main()
{
  int numOfArray;
  cin >> numOfArray; cin.ignore();

  Card *pCard = new Card[numOfArray];
  for (int i = 0; i < numOfArray; i++)
  {
    int v;
    char s;
    cin >> s; cin >> v;
    pCard[i].set(v, s);
  }

  Card *pCard2 = new Card[numOfArray];
  for (int i = 0; i < numOfArray; i++)
  {
    pCard2[i] = pCard[i];
  }

#if DEBUG
  DPRINTF("numOfArray %d\n", numOfArray);
  for (int i = 0; i < numOfArray; i++)
    DPRINTF("%d: %c %d\n", i, pCard[i].getCharFromSuit(), pCard[i].getValue());
#endif // DEBUG

  int start = 0;
  int end = numOfArray - 1;

  // Not sure sorted stably or not
  // Run time : log N
  quickSort(pCard, start, end);

#if 0
  // Bubble sort sort out stably
  // Run time : N^2
  bubbleSort(pCard2, numOfArray);
#endif

  // stable sort with C STL
  // Run time : N((logN)^2)
  stable_sort(&pCard2[0], &pCard2[numOfArray], &comp_card);

  // Check if the quick sorted array is stable or not
  bool stable = true;
  for (int i = 0; i < numOfArray; i++)
  {
    if (pCard[i] != pCard2[i])
    {
      stable = false;
      break;
    }
  }

  if (stable) cout << "Stable" << endl;
  else        cout << "Not stable" << endl;

  for (int i = 0; i < numOfArray; i++)
    printf("%c %d\n", pCard[i].getCharFromSuit(), pCard[i].getValue());

#if DEBUG
  DPRINTF("stable_sort sort -------------\n");
  for (int i = 0; i < numOfArray; i++)
    DPRINTF("%c %d\n", pCard2[i].getCharFromSuit(), pCard2[i].getValue());
#endif // DEBUG

  return 0;
}

