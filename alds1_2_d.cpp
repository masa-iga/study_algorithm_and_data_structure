#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stack>
using namespace std;

//#define DEBUG (1)

#ifdef DEBUG
#define DPRINT(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

void  getGarray(int arraySize, int *&pG, int &gSize);
void  insertionSort(int array[], const int arraySize, const int d);
void printArray(const int array[], const int arraySize);

int g_cnt = 0;

int main() {
	int arraySize;
	cin >> arraySize; cin.ignore();
	assert(arraySize > 0);

	int array[arraySize];
	for (int i = 0; i < arraySize; i++)
		cin >> array[i];

#ifdef DEBUG
	DPRINT("INPUT: ");
	printArray(array, arraySize);
#endif
	// shell sort
	int *pG, gSize;
	getGarray(arraySize, pG, gSize);

	cout << gSize << endl;
	printArray(pG, gSize);

	for (int i = 0; i < gSize; i++)
		insertionSort(array, arraySize, pG[i]);

	cout << g_cnt << endl;
	for (int i = 0; i < arraySize; i++)
		cout << array[i] << endl;

	delete [] pG;
	return 0;
}

void getGarray(int arraySize, int *&pG, int &gSize) {
	stack<int> s;
	int i = 1;
	while (i <= arraySize) {
		s.push(i);
		i *= 2;
	}
	gSize = s.size();
	pG = new int[gSize];
	for (int i = 0; i < gSize; i++) {
		pG[i] = s.top();
		s.pop();
	}
#ifdef DEBUG
	DPRINT("gSize: %d\n", gSize);
	DPRINT("pG addr %p\n", pG);
	DPRINT("pG array: ");
	printArray(pG, gSize);
#endif
}

void insertionSort(int array[], const int arraySize, const int d) {
	DPRINT("d: %d\n", d);
	for (int i = d; i < arraySize; i++){
		int j = i;
		int v = array[j];
		while (j-d >= 0 && v < array[j-d]) {
			array[j] = array[j-d];
			j -= d;
			g_cnt++;
		}
		array[j] = v;
	}
#ifdef DEBUG
	DPRINT("array: ");
	printArray(array, arraySize);
#endif
	return ;
}

void printArray(const int array[], const int arraySize) {
	for (int i = 0; i < arraySize; i++) {
		if (i > 0)	cout << " ";
		cout << array[i];
	}
	cout << endl;
}

