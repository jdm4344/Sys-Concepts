// HW4_V2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>
#include <Windows.h>

using namespace std;

int numChairs = 0;
int numClients = 0;
int numBarbers = 0;
int waiting = 0;

vector<thread> clientThreads;
vector<thread> barberThreads;

struct CustomSem {
	mutex mtx;
	condition_variable cv;
	int count;

	CustomSem(int ct) : count(ct)
	{
		count = ct;
	}

	inline void up(int threadId)
	{
		unique_lock<mutex> lock(mtx);
		count++;
		cv.notify_one();
	}

	inline void down(int threadId)
	{
		unique_lock<mutex> lock(mtx);

		while (count == 0)
		{
			cv.wait(lock);
		}
		count--;
	}	
};

void cutHair(int id)
{
	//cout << "Barber " << id << " is cutting hair" << endl;
	printf("Barber %d is cutting hair\n", id);
}

void getHaircut(int id)
{
	//cout << "Customer " << id << " is getting their hair cut" << endl;
	printf("Customer %d is getting their hair cut\n", id);
}

void Barber(int id, CustomSem& mutexSem, CustomSem& customers, CustomSem& barbers) {
	while (true)
	{
		customers.down(id); // down(customers); // go to sleep if # of customers = 0
		mutexSem.down(id); // dowm(mutex); // acquire access to 'waiting'
		waiting--;
		barbers.up(id); // up(barbers); // one barber is now ready to cut hair
		mutexSem.up(id); // up(mutex); // release waiting;
		cutHair(id); // cut hair
	}
}


void Customer(int id, CustomSem& mutexSem, CustomSem& customers, CustomSem& barbers)
{
	mutexSem.down(id); // down(mutex);
	if (waiting < numChairs)
	{
		waiting++; // increment count of waiting customers
		customers.up(id); // up(customers); // wake up barber if necessary
		mutexSem.up(id); // up(mutex); // release access to 'waiting'
		barbers.down(id); // down(barbers); // go to sleep if # of free barbers = 0
		getHaircut(id); // be seated and served
	}
	else
	{
		mutexSem.up(id); // up(mutex); // shop is full, do not wait
		//cout << "Not enough chairs, customer " << id << " leaves" << endl;
		printf("Not enough chairs, customer %d leaves\n", id);
	}
}

int main()
{
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

	CustomSem customers(numClients);
	CustomSem barbers(numBarbers);
	CustomSem mutexSem(numChairs);

	// Create threads
	for (int i = 0; i < numBarbers; i++) {
		barberThreads.push_back(thread(Barber, i, ref(mutexSem), ref(customers), ref(barbers)));
	}

	for (int i = 0; i < numClients; i++) {
		clientThreads.push_back(thread(Customer, i, ref(mutexSem), ref(customers), ref(barbers)));
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

