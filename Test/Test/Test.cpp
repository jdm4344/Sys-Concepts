// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

typedef int semaphore;

semaphore customers = 0;
semaphore barbers = 0;
semaphore mutex = 1;
int chairs = 0;
int waiting = 0;

vector<thread> clientThreads;
vector<thread> barberThreads;

void up(semaphore &sem)
{
	sem++;
}

void down(semaphore &sem)
{
	if (sem > 0)
	{
		sem--;
	}
	else if (sem == 0)
	{
		this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void cutHair()
{
	cout << "Barber " << this_thread::get_id << " is cutting hair" << endl;
}

void getHaircut()
{
	cout << "Customer " << this_thread::get_id << " is getting their hair cut" << endl;
}

static void Barber()
{
	while (true)
	{
		down(customers); // go to sleep if # of customers = 0
		down(mutex); // acquire access to 'waiting'
		waiting--;
		up(barbers); // one barber is now ready to cut hair
		up(mutex); // release waiting;
		cutHair(); // cut hair
	}
}

static void Customer()
{
	if (waiting < chairs)
	{
		down(mutex);
		waiting++; // increment count of waiting customers
		up(customers); // wake up barber if necessary
		up(mutex); // release access to 'waiting'
		down(barbers); // go to sleep if # of free barbers = 0
		getHaircut(); // be seated and served
	}
	else
	{
		up(mutex); // shop is full, do not wait
		cout << "Not enough chairs, customer " << this_thread::get_id << " leaves" << endl;
	}
}

int main()
{
	// Get the parameters from the user
	while (chairs <= 0)
	{
		cout << "Enter number of chairs: ";
		cin >> chairs;
	}

	while (customers <= 0)
	{
		cout << "Enter number of clients: ";
		cin >> customers;
	}

	while (barbers <= 0)
	{
		cout << "Enter number of barbers: ";
		cin >> barbers;
	}

	// Create threads
	for (int i = 0; i < barbers; i++) {
		//barberThreads.push_back(thread(Barber, i, std::ref(mutex)));
		barberThreads.push_back(thread([i, ref(mutex)] { Barber(); }));
	}

	for (int i = 0; i < customers; i++) {
		//clientThreads.push_back(thread(Customer, i, std::ref(mutex)));
		clientThreads.push_back(thread([i, ref(mutex)] { Customer(); }));
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

