#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

#define DEBUG (1)

#ifdef DEBUG
#define DPRINT(...)  printf("D(L%d)%s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

void printArray(int ary[], int size);

int main() {
  int size;
  cin >> size; cin.ignore();
  assert(size > 0);

  int input[size];
  for (int i = 0; i < size; i++) {
    cin >> input[i];
  }
  printArray(input, size);

  for (int i = 1; i < size; i++) {
    int v = input[i];
    int j = i - 1;
    while (j >= 0 && (input[j] > v)) {
      input[j+1] = input[j];
      j--;
    }
    input[j+1] = v;
    printArray(input, size);
  }

  return 0;
}

void printArray(int ary[], int size) {
  for (int i = 0; i < size-1; i++)
    cout << ary[i] << " ";
  cout << ary[size-1] << endl;;
}
