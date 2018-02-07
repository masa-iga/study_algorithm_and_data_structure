#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>
#include <algorithm>
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

int g_cost = 0;

void swap(int array[], int i0, int i1)
{
  int temp = array[i1];
  array[i1] = array[i0];
  array[i0] = temp;
}


int findIndexOf(int val, const int array[], int numOfArray)
{
  for (int i = 0; i < numOfArray; i++)
    if (array[i] == val)
      return i;

  // if cannot find the val, call assertion error.
  assert(0);

  return -1;
}


// The formula is : Sigma(Wi) + min(Wi) * (N-2)
int calcCostSimple(const int array[], const vector<int> &l)
{
  int s = 0;
  int minimum = INT_MAX;
  for (uint32_t i = 0; i < l.size(); i++)
  {
    int idx = l[i];
    s += array[idx];
    minimum = min(minimum, array[idx]);
  }

  DPRINTF("s %d  minimum %d\n", s, minimum);

  return s + minimum * (l.size() - 2);
}


// The formula is : Sigma(Wi) + min(Wi) + x * (N+1)
int calcCostSwap(const int array[], const vector<int> &l, int x)
{
  if (array == nullptr || l.size() < 1 || x < 0)
  {
    assert(1);
    return INT_MAX;
  }

  int s = 0;
  int minimum = INT_MAX;
  for (uint32_t i = 0; i < l.size(); i++)
  {
    int idx = l[i];
    s += array[idx];
    minimum = min(minimum, array[idx]);
  }

  DPRINTF("s %d  minimum %d  x %d\n", s, minimum, x);

  return s + minimum + x * (l.size() + 1);
}


int findMinOutsideLoop(vector<int> indices, int max, const int sortedArray[])
{
  sort(indices.begin(), indices.end()); 

  for (int i = 0; i < max; i++)
    if (indices[i] != i)
      return sortedArray[i];

  // the case l and sortedArray are the same
  return INT_MAX;
}


int main()
{
  int numOfArray;
  cin >> numOfArray; cin.ignore();

  int array[numOfArray];
  for (int i = 0; i < numOfArray; i++)
    cin >> array[i];

#if DEBUG
  for (int i = 0; i < numOfArray; i++)
    if (i == 0) DPRINTF("%2d", array[i]);
    else        printf(" %2d", array[i]);
  cout << endl;
#endif // DEBUG

  int sortedArray[numOfArray];
  for (int i = 0; i < numOfArray; i++)
    sortedArray[i] = array[i];

  sort(sortedArray, sortedArray + numOfArray);

#if DEBUG
  for (int i = 0; i < numOfArray; i++)
    if (i == 0) DPRINTF("%2d", sortedArray[i]);
    else        printf(" %2d", sortedArray[i]);
  cout << endl;
#endif // DEBUG

  // First, find a loop where each element is sorted
  //   Suppose array of { 2 1 8 10 7 9 }
  //   then, sorted array should be { 1 2 7 8 9 10 }
  //   In this case, we could find two loops;
  //     Loop 1 : { 2 1 } - { 1 2 }
  //     Loop 2 : { 8 10 7 9 } - { 7 8 9 10 }

  bool visited[numOfArray];
  fill(visited, visited + numOfArray, 0);

  vector<vector<int>> loops;

  for (int i = 0; i < numOfArray; i++)
  {
    if (visited[i])
      continue;

    int idx = i;
    vector<int> loop;

    while (!visited[idx])
    {
      loop.push_back(idx);
      visited[idx] = true;

      int num = array[idx];
      idx = findIndexOf(num, sortedArray, numOfArray);
    }

    loops.push_back(loop);
  }

  // Let's calculate a cost when we will need to sort out
  // Note that we have to check two cases in a way of sorting, and compare of which is light
  // Take a look at loop 2 case;
  //   { 8 10 7 9 } will sorted, and it becomes { 7 8 9 10 }
  //   The cost should be 48;
  //   The general formula will be : Sigma(Wi) + min(Wi) * (N-2)
  // 
  //   Be careful that we could swap with a number of another loop;
  //   Let's say swap 1 of loop 1 with 7 of loop 2
  //   and sort { 8 10 1 9 }, and it will be { 1 8 9 10 }
  //   then swap again, 1 and 7, finally the array will be sorted properly as { 7 8 9 10 }
  //   In this case, the cost should be 45
  //   The general formula should be : Signma(Wi) + min(Wi) + x * (N+1)

  for (uint32_t i = 0; i < loops.size(); i++)
  {

    const vector<int> l = loops[i];

#if DEBUG
    DPRINTF("Loop %d : ",i);
    for (uint32_t j = 0; j < l.size(); j++) {
      printf(" %d", array[l[j]]);
    }
    cout << endl;
#endif

    if (l.size() < 2)
      continue; // means the number will be alredy sorted, no need to consider

    int costA, costB;
    costA = calcCostSimple(array, l);
    int x = findMinOutsideLoop(l, numOfArray, sortedArray);
    if (x == INT_MAX)
      costB = INT_MAX;
    else
      costB = calcCostSwap(array, l, x);

    g_cost += min(costA, costB);

    DPRINTF("Loop %d : Simple %d  Swap %d  g_cost %d\n", i, costA, costB, g_cost);
  }

  cout << g_cost << endl;

  return 0;
}

