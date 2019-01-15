// SillyCPPTrick.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>


using namespace std;

int mysillyfunc(int a, int b, int c) {

	cout << "a is " << a << " b is " << b << " c is " << c << endl;
	return a + b + c;
}

int main()
{
	int val = 10;
	int retval = 0;
	
	retval = mysillyfunc(val++, val++, val++);
	cout << "retval is " << retval << endl;
	cout << "val is " << val << endl;

	val = 10;
	retval = 0;

	retval = mysillyfunc(++val, ++val, ++val);
	cout << "retval is " << retval << endl;
	cout << "val is " << val << endl;
	// DO NOT USE THE FOLLOWING IF PROFILING!
	system("pause");
}

