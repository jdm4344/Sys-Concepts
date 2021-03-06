// Test2.cpp : Defines the entry point for the console application.
//

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

void cutHair(int id)
{
	//cout << "Barber " << id << " is cutting hair" << endl;
	printf("Barber %d is cutting hair\n", id);
	Sleep(1);
}

void getHaircut(int id)
{
	//cout << "Customer " << id << " is getting their hair cut" << endl;
	printf("Customer %d is getting their hair cut\n", id);
}

void Barber(int id, HANDLE& mutexSem, HANDLE& customers, HANDLE& barbers) {
	while (true)
	{
		//customers.down(id); // down(customers); // go to sleep if # of customers = 0
		//mutexSem.down(id); // dowm(mutex); // acquire access to 'waiting'
		//waiting--;
		//barbers.up(id); // up(barbers); // one barber is now ready to cut hair
		//mutexSem.up(id); // up(mutex); // release waiting;
		//cutHair(id); // cut hair

		WaitForSingleObject(customers, 0L);
		WaitForSingleObject(mutexSem, 0L);
		waiting--;
		ReleaseSemaphore(barbers, 1, NULL);
		ReleaseSemaphore(mutexSem, 1, NULL);
		cutHair(id);
	}
}

void Customer(int id, HANDLE& mutexSem, HANDLE& customers, HANDLE& barbers)
{
	//mutexSem.down(id); // down(mutex);
	WaitForSingleObject(mutexSem, 0L);
	if (waiting < numChairs)
	{
		//waiting++; // increment count of waiting customers
		//customers.up(id); // up(customers); // wake up barber if necessary
		//mutexSem.up(id); // up(mutex); // release access to 'waiting'
		//barbers.down(id); // down(barbers); // go to sleep if # of free barbers = 0
		//getHaircut(id); // be seated and served
		
		waiting++;
		ReleaseSemaphore(customers, 1, NULL);
		ReleaseSemaphore(mutexSem, 1, NULL);
		WaitForSingleObject(barbers, 0L);
		getHaircut(id);
	}
	else
	{
		//mutexSem.up(id); // up(mutex); // shop is full, do not wait
		ReleaseSemaphore(mutexSem, 1, NULL);

		//cout << "Not enough chairs, customer " << id << " leaves" << endl;
		printf("Not enough chairs, customer %d leaves\n", id);
	}
}

int main()
{
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
	}

	for (auto& thread : clientThreads) {
		thread.join();
	}

    return 0;
}

