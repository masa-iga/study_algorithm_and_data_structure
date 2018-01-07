#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
using namespace std;

#define DEBUG (1)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

bool binarySearch(const int array[], const int start, const int end, const int val)
{
  DPRINTF("START %d  END %d\n", start, end);

  if (start > end)
    return false;

  int mid = (start + end) / 2;

  if (val == array[mid])
    return true;
  else if (val > array[mid])
    return binarySearch(array, mid + 1, end, val);
  else // (val < array[mid]
    return binarySearch(array, start, mid - 1, val);
}


int main() {
  int numOfS, numOfT;

  cin >> numOfS; cin.ignore();
  int s[numOfS];
  for (int i = 0; i < numOfS; i++) {
    cin >> s[i];
  }


  cin >> numOfT; cin.ignore();
  int t[numOfT];
  for (int i = 0; i < numOfT; i++) {
    cin >> t[i];
  }

#if DEBUG
  DPRINTF("ARRAY S :");
  for (int i = 0; i < numOfS; i++)
    cout << " " << s[i];
  cout << endl;

  DPRINTF("ARRAY T :");
  for (int i = 0; i < numOfT; i++)
    cout << " " << t[i];
  cout << endl;
#endif

  // We can assume that all elements in S is sorted in asceding order
  int count = 0;
  for (int i = 0; i < numOfT; i++) {
    if (binarySearch(s, 0, numOfS - 1, t[i]))
      count++;
  }
  cout << count << endl;
  return 0;
}

