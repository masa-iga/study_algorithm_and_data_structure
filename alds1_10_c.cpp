#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <vector>
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


class LongestCommonSubsequence {
public:
  LongestCommonSubsequence(string s0, string s1) :
    s0_(s0),
    s1_(s1)
    {
      size_ = s0_.length() * s1_.length();
      pLongest_ = new int [size_];
      for (int i = 0; i < size_; i++)
        pLongest_[i] = kInvalid;
    }

  ~LongestCommonSubsequence()
  {
    if (pLongest_)
      free(pLongest_);
  }

  int getLCSlen();
  void printLength();

  const int kInvalid = -1;

private:
  string s0_;
  string s1_;
  int *pLongest_;
  int size_;

  int getIndex(int x, int y);
  int getLCSlenOf(int n0, int n1);
};

/*********************************
Input:
  x[0], x[1], x[2], ..., x[n-1]
  y[0], y[1], y[2], ..., y[m-1]

Algorithm:
  if x[n-1] == y[m-1]
    LCS(n-1, m-1) = LCS(n-2,m-2) + 1

  if x[n-1] != y[m-1]
    LCS(n-1, m-1) = max(LCS(n-2, m-1), LCS(n-1, m-2))

  LCS(0, *) = LCS(*, 0) = 0;

  a b c b d a b
  b d c a b a
*********************************/

int LongestCommonSubsequence::getLCSlen()
{
  int n0 = s0_.length() - 1;
  int n1 = s1_.length() - 1;
  return getLCSlenOf(n0, n1);
}


int LongestCommonSubsequence::getLCSlenOf(int n0, int n1)
{
  DPRINTF("n0 %d  n1 %d\n", n0, n1);

  assert(-1 <= n0 && n0 < static_cast<int>(s0_.length()));
  assert(-1 <= n1 && n1 < static_cast<int>(s1_.length()));

  if (n0 < 0  || n1 < 0)
    return 0;

  const int idx = getIndex(n0, n1);

  if (pLongest_[idx] != kInvalid)
    return pLongest_[idx];

  if (s0_[n0] == s1_[n1])
    pLongest_[idx] = getLCSlenOf(n0 - 1, n1 - 1) + 1;
  else
    pLongest_[idx] = max(getLCSlenOf(n0 - 1, n1), getLCSlenOf(n0, n1 - 1));

  return pLongest_[idx];
}


int LongestCommonSubsequence::getIndex(int x, int y)
{
  return y * s0_.length() + x;
}


void LongestCommonSubsequence::printLength()
{
  for (int i = 0; i < s0_.length(); i++)
    for (int j = 0; j < s1_.length(); j++)
      DPRINTF("pLongest_[%d][%d] %d\n", i, j, pLongest_[getIndex(i, j)]);
}


int main()
{
  int numOfPair;
  cin >> numOfPair; cin.ignore();

  DPRINTF("num of pair %d\n", numOfPair);

  for (int i = 0; i < numOfPair; i++)
  {
    string str0, str1;
    cin >> str0 >> str1; cin.ignore();

    DPRINTF("str0 %s  str1 %s\n", str0.c_str(), str1.c_str());

    LongestCommonSubsequence lcs(str0, str1);
    cout << lcs.getLCSlen() << endl;

#if (1) && DEBUG
    lcs.printLength();
#endif
  }

  return 0;
}

