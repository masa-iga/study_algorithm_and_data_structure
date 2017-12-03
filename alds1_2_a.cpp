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

void swap(int &a, int &b);
void printAns(int ary[], int size, int numOfExchange);

int main() {
	int size;
	cin >> size; cin.ignore();
	assert(size > 0);

	int ary[size];
	for (int i=0; i < size; i++)
		cin >> ary[i];

	int i = 0;
	int numOfExchange = 0;
	bool done = 0;
	while (!done) {
		done = 1;
		for (int j = size-1; j > i; j--) {
			if (ary[j] < ary[j-1]) {
				swap(ary[j], ary[j-1]);
				done = 0;
				numOfExchange++;
			}
		}
		i++;
	}
	printAns(ary, size, numOfExchange);
	return 0;
}

void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

void printAns(int ary[], int size, int numOfExchange) {
	for (int i = 0; i < size; i++) {
		if (i > 0) cout << " ";
		cout << ary[i];
	}
	cout << endl;
	cout << numOfExchange << endl;
}
