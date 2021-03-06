// HW4.cpp : Defines the entry point for the console application.
// Jordan Machalek
// Implementation of Sleeping Barbers IPC Problem

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <Windows.h>
using namespace std;

// Globals
int numChairs = 0;
int numClients = 0;
int numBarbers = 0;
int waiting = 0;
int remainingCustomers = 0;

// Represents a barber cutting hair
void cutHair(int id)
{
	printf("Barber %d is cutting hair\n", id);
	Sleep(1);
}

// Reperesents a customer getting their hair cut
void getHaircut(int id)
{
	printf("Customer %d is getting their hair cut\n", id);
	remainingCustomers--; // decrement the remaining customers so the barbers will eventually stop
}

// Represents a single barber thread
void Barber(int id, HANDLE& mutexSem, HANDLE& customers, HANDLE& barbers) {
	while (remainingCustomers > 0)
	{
		WaitForSingleObject(customers, 0L); // go to sleep if # of customers = 0
		WaitForSingleObject(mutexSem, 0L); // acquire access to 'waiting'
		waiting--;
		ReleaseSemaphore(barbers, 1, NULL); // one barber is now ready to cut hair
		ReleaseSemaphore(mutexSem, 1, NULL); // release waiting;
		cutHair(id);
	}
}

// Reperesents a single customer thread
void Customer(int id, HANDLE& mutexSem, HANDLE& customers, HANDLE& barbers)
{
	WaitForSingleObject(mutexSem, 0L);

	if (waiting < numChairs)
	{
		waiting++; // increment count of waiting customers
		printf("Customer %d is waiting for a haircut\n", id);
		if (ReleaseSemaphore(customers, 1, NULL) == true) // wake up barber if necessary
		{
			printf("Customer %d woke up the barber\n", id);
		} // else, the barber was awake and ReleaseSemaphore() returned false
		ReleaseSemaphore(mutexSem, 1, NULL); // release access to 'waiting'
		WaitForSingleObject(barbers, 0L); // go to sleep if # of free barbers = 0
		getHaircut(id); // be seated and served
	}
	else
	{
		ReleaseSemaphore(mutexSem, 1, NULL); // shop is full, do not wait

		printf("Not enough chairs, customer %d leaves\n", id);
		remainingCustomers--; // decrement the remaining customers so the barbers will eventually stop
	}
}

int main()
{
	// Thread storage
	vector<thread> clientThreads;
	vector<thread> barberThreads;

	// Get the parameters from the user
	while (numChairs <= 0)
	{
		cout << "Enter number of chairs: ";
		cin >> numChairs;
	}

	while (numClients <= 0)
	{
		cout << "Enter number of clients: ";
		cin >> numClients;
	}

	remainingCustomers = numClients;

	while (numBarbers <= 0)
	{
		cout << "Enter number of barbers: ";
		cin >> numBarbers;
	}

	// Create Semaphores
	HANDLE hCustomers = CreateSemaphoreA(
		NULL,
		numClients,
		numClients,
		"customers"
	);

	HANDLE hBarbers = CreateSemaphoreA(
		NULL,
		numBarbers,
		numBarbers,
		"barbers"
	);

	HANDLE hMutex = CreateSemaphoreA(
		NULL,
		0,
		1,
		"mutex"
	);

	// Create threads
	for (int i = 0; i < numBarbers; i++) {
		barberThreads.push_back(thread(Barber, i, ref(hMutex), ref(hCustomers), ref(hBarbers)));
	}

	for (int i = 0; i < numClients; i++) {
		clientThreads.push_back(thread(Customer, i, ref(hMutex), ref(hCustomers), ref(hBarbers)));
	}

	// Start threads
	for (auto& thread : barberThreads) {
		thread.join();
		Sleep(1);
	}

	for (auto& thread : clientThreads) {
		thread.join();
		Sleep(1);
	}

	// Done!
	printf("No more customers, the shop is closing for the day\n");

	return 0;
}