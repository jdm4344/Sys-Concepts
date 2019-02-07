// forLoopsFaster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <string>
#include <ppl.h>

using namespace Concurrency;
using namespace std;

#define SIZE 6000
int intArray[SIZE][SIZE];
size_t sum = 0;

// vanilla doing each row completely in the tight loop - correct/fast way
void array1() {
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			intArray[i][j] = 5;
			sum += intArray[i][j];
		}
	}
}

// vanilla doing each col completely in the tight loop - wrong/slow way
void array2() {
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			intArray[j][i] = 6;
			sum += intArray[j][i];
		}
	}
}

// using memset to set the mem to 0
// won't set things correctly to other values
// If we use a value other than zero, we are actually setting the byte pattern
// Set a breakpoint and try different patterns, chars and ints, how is it treating the
// second param?
void array3() {
	memset(intArray, 0, sizeof(int)*SIZE*SIZE);
}

// using a combination of memcpy and unrolling the loop
void array4() {
	unsigned int value = 8;
	int temp[SIZE];

	// Fill 1 row with values
	for (size_t j = 0; j < SIZE; j++) {
		temp[j] = value;
	}

	// Use memcpy to copy values of first row to remaining rows
	for (size_t i = 0; i < SIZE; i++) {
		memcpy(intArray[i], temp, SIZE * sizeof(int));
	}
}

void array5() {
	unsigned int value = 5;
	size_t size = SIZE;
	int temp[SIZE];
	// can't do anon funcs in viz studio 2010
	// This is using the parallel
	// There are a lot of partitioner types
	// auto_partitioner - Default (range stealing/load balancing, per iterate cancellation)
	// simple_partitioner - Divide into chunks / each chunk has at least the number of interations specified by chunk size
	// static_partitioner - Divide into as many chunks as available to the scheduler 
	// affinity_partitioner
	parallel_for(size_t(0), (size_t)size, [&](size_t i) {
		temp[i] = value;
	}, auto_partitioner()); //static_partitioner());

	parallel_for(size_t(0), (size_t)size, [&](size_t i) {
		memcpy(intArray[i], temp, SIZE * sizeof(int));
	}, auto_partitioner()); //static_partitioner());
}

// tests of the different array init functions
int main() {
	array1();
	printf("%ul \r\n", sum);
	sum = 0;
	array2();
	printf("%ul \r\n", sum);
	array5();
	array3();
	array4();
	//system("pause");
}