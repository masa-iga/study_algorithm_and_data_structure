#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
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


struct MtxSize {
  int numOfRow;
  int numOfColumn;
};


struct MtxInfo {
  int minMultiply;
  int numOfRow;
  int numOfColumn;
};


const int kMaxSize = 100;
const int kInvalid = -1;
MtxInfo minArrays[kMaxSize][kMaxSize];


inline void setMtxInfo(int idx0, int idx1, const MtxInfo &mtx)
{
  minArrays[idx0][idx1] = mtx;
}

inline MtxInfo getMtxInfo(int idx0, int idx1)
{
  return minArrays[idx0][idx1];
}

inline bool isValid(int idx0, int idx1)
{
  const MtxInfo &mtx = minArrays[idx0][idx1];
  return mtx.minMultiply != kInvalid ? true : false;
}

/*
   find minimum matrix chain multiplication between matrices[start] ... matrices[end-1]
 */
MtxInfo getMinScalarMultiply(const MtxSize matrices[], int start, int end)
{
  DPRINTF("start %d  end %d\n", start, end);

  assert(start < end);

  if (isValid(start, end))
    return getMtxInfo(start, end);

  if (start + 1 == end)
  {
    MtxInfo mtx;

    mtx.minMultiply = 0;
    mtx.numOfRow = matrices[start].numOfRow;
    mtx.numOfColumn = matrices[start].numOfColumn;

    setMtxInfo(start, end, mtx);

    return getMtxInfo(start, end);
  }


  MtxInfo minMtx;
  minMtx.minMultiply = INT_MAX;

  for (int i = start + 1; i < end; i++)
  {
    MtxInfo mtx0 = getMinScalarMultiply(matrices, start, i);
    MtxInfo mtx1 = getMinScalarMultiply(matrices, i, end);

    MtxInfo mtx;

    mtx.minMultiply = mtx0.minMultiply + mtx1.minMultiply + mtx0.numOfRow * mtx0.numOfColumn * mtx1.numOfColumn;
    mtx.numOfRow = mtx0.numOfRow;
    mtx.numOfColumn = mtx1.numOfColumn;

#if DEBUG
    DPRINTF("start %d  end %d  i %d : mtx.minMultiply %d  min.minMultiply %d\n",
        start, end, i, mtx.minMultiply, minMtx.minMultiply);
    DPRINTF("  mtx0.minMultiply %d  mtx1.minMultiply %d\n",
        mtx0.minMultiply, mtx1.minMultiply);
#endif

    if (minMtx.minMultiply > mtx.minMultiply)
      minMtx = mtx;
  }

  DPRINTF("start %d  end %d  multiply %d\n", start, end, minMtx.minMultiply);

  setMtxInfo(start, end, minMtx);

  return getMtxInfo(start, end);
}


int calcMinScalarMultiply(const MtxSize matrices[], int num)
{
  if (num <= 1)
    return 0;

  MtxInfo mtx = getMinScalarMultiply(matrices, 0, num);

  return mtx.minMultiply;
}


int main() {

  // init
  for (int i = 0; i < kMaxSize; i++)
    for (int j = 0; j < kMaxSize; j++)
      minArrays[i][j].minMultiply = kInvalid;

  int numOfMatrix;
  cin >> numOfMatrix; cin.ignore();

  MtxSize  matrices[numOfMatrix];
  for (int i = 0; i < numOfMatrix; i++)
  {
    cin >> matrices[i].numOfRow >> matrices[i].numOfColumn; cin.ignore();
  }

#if DEBUG
  for (int i = 0; i < numOfMatrix; i++)
  {
    DPRINTF("MtxSize %d: %d %d\n", i, matrices[i].numOfRow, matrices[i].numOfColumn);
  }
#endif

  cout << calcMinScalarMultiply(matrices, numOfMatrix) << endl;

  return 0;
}


