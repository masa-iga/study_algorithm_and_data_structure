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
void printAns(int ary[], int size, int count);

int main() {
	int size;
	cin >> size; cin.ignore();
	assert(size > 0);

	int ary[size];
	for (int i=0; i < size; i++)
		cin >> ary[i];

	int count = 0;
	for (int i=0; i < size-1; i++) {
		int minIdx = i;
		for (int j = i+1; j < size; j++) {
			if (ary[minIdx] > ary[j])
				minIdx = j;
		}
		if (i != minIdx) {
			swap(ary[i], ary[minIdx]);
			count++;
		}
	}

	printAns(ary, size, count);
	return 0;
}

void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

void printAns(int ary[], int size, int count) {
	for (int i=0; i < size; i++) {
		if (i > 0)	cout << " ";
		cout << ary[i];
	}
	cout << endl;
	cout << count << endl;
}
