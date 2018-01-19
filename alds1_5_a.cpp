#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

#define DEBUG (1)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

bool solve(const int i, const int max_i, const int array[], const int val)
{
  if (val == 0)
    return true;
  else if (i > max_i || val < 0)
    return false;

  bool left = solve(i + 1, max_i, array, val);
  if (left == true)
    return true;

  return solve(i + 1, max_i, array, val - array[i]);
}


int main()
{
  int numOfArrayA;
  cin >> numOfArrayA; cin.ignore();

  int arrayA[numOfArrayA];
  for (int i = 0; i < numOfArrayA; i++) cin >> arrayA[i];

  int numOfArrayQ;
  cin >> numOfArrayQ; cin.ignore();

  int arrayQ[numOfArrayQ];
  for (int i = 0; i < numOfArrayQ; i++) cin >> arrayQ[i];

#if DEBUG
  for (int i = 0; i < numOfArrayA; i++)
  {
    if (i == 0) { DPRINTF("Array A: %d", arrayA[i]); }
    else        { printf(" %d", arrayA[i]); }
  }
  cout << endl;

  for (int i = 0; i < numOfArrayQ; i++)
  {
    if (i == 0) { DPRINTF("Array Q: %d", arrayQ[i]); }
    else        { printf(" %d", arrayQ[i]); }
  }
  cout << endl;
#endif

  for (int i = 0; i < numOfArrayQ; i++)
  {
    const int val = arrayQ[i];

    if (solve(0, numOfArrayA - 1, arrayA, val))
      cout << "yes" << endl;
    else
      cout << "no" << endl;
  }

  return 0;
}
