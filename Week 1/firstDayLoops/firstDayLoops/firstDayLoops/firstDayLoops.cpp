// firstDayLoops.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>


using namespace std;

#define SIZE 7000
int intArray[SIZE][SIZE];
size_t sum = 0;


// vanilla doing each row completely in the tight loop
void array1() {
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			intArray[i][j] = 5;
		//	sum += intArray[i][j];
		}
	}
}

// vanilla doing each col completely in the tight loop
void array2() {
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			intArray[j][i] = 5;
		//	sum += intArray[i][j];
		}
	}
}

int main()
{
	array1();
	cout << sum  << endl;
	sum = 0;
	array2();
	cout << sum << endl;
	// DO NOT USE THE FOLLOWING IF PROFILING!
	system("pause");
}

