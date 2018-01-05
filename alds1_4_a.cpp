#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
using namespace std;

#define DEBUG (0)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

void mySort(int *array, int start, int end);

int main()
{
  const int MAX_S = 10000;
  const int MAX_T = 500;
  int numOfS, numOfT;
  int s[MAX_S], t[MAX_T];

  cin >> numOfS; cin.ignore();
  for (int i = 0; i < numOfS; i++) {
    cin >> s[i];
  }

  cin >> numOfT; cin.ignore();
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

  if (0) {
    sort(s, s + numOfS);
    sort(t, t + numOfT);
  } else {
    mySort(s, 0, numOfS - 1);
    mySort(t, 0, numOfT - 1);
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

  int cnt = 0;
  int i = 0, j = 0;
  while (i < numOfS && j < numOfT)
  {
    if (s[i] == t[j])
    {
      DPRINTF("MATCH : %d\n", s[i]);
      cnt++;
      i++;
      j++;
    }
    else if (s[i] > t[j])
    {
      j++;
    }
    else // s[i] < t[j]
    {
      i++;
    }
  }

  cout << cnt << endl;

  return 0;
}

void mySort(int *array, int start, int end)
{ // quick sort
  if (start >= end)
    return;

  DPRINTF("START %d  END %d\n", start, end);
  int mid = (start + end) / 2;
  int pivot = array[mid];
  int left = start, right = end;

  while (left <= right)
  {
    while (array[left] < pivot) left++;
    while (array[right] > pivot) right--;

    if (left <= right)
    {
      int temp = array[left];
      array[left] = array[right];
      array[right] = temp;
      left++, right--;
    }
  }

#if DEBUG
  DPRINTF("ARRAY ");
  for (int i = start; i < end; i++)
    cout << " " << array[i];
  cout << endl;
#endif

  mySort(array, start, left - 1);
  mySort(array, left, end);

  return ;
}

