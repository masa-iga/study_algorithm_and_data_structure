#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace std;

#define DEBUG (1)

#ifdef DEBUG
#define DPRINT(...)  printf("D(L%d)%s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

int main(){
  int n;
  cin >> n; cin.ignore();

  if (n < 2) {
    cout << 0 << endl;
    return 0;
  }

  int ary[n];
  for (uint32_t i = 0; i < n; i++) {
    cin >> ary[i];
    DPRINT("ary[%d] = %d\n", i, ary[i]);
  }

  int maxProf = ary[1] - ary[0];
  int minRi = ary[0];
  for (uint32_t i = 2; i < n; i++) {
    if (minRi > ary[i-1])
      minRi = ary[i-1];

    int profit = ary[i] - minRi;
    if (maxProf < profit)
      maxProf = profit;
  }

  cout << maxProf << endl;
  return 0;
}
