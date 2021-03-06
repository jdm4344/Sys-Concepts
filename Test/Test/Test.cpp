// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
using namespace std;

typedef int semaphore;

semaphore customers = 0;
semaphore barbers = 0;
semaphore mutexSem = 1;
int chairs = 0;
int waiting = 0;

vector<thread> clientThreads;
vector<thread> barberThreads;

void up(semaphore &sem)
{
	mutex lock;

	unique_lock<mutex> l(lock);

	sem++;

	l.unlock();
}

void down(semaphore &sem)
{
	mutex lock;

	unique_lock<mutex> l(lock);

	if (sem > 0)
	{
		sem--;
	}
	else if (sem == 0)
	{
		this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	l.unlock();
}

void cutHair(int id)
{
	cout << "Barber " << id << " is cutting hair" << endl;
}

void getHaircut(int id)
{
	cout << "Customer " << id << " is getting their hair cut" << endl;
}

static void Barber(int id)
{
	while (true)
	{
		down(customers); // go to sleep if # of customers = 0
		down(mutexSem); // acquire access to 'waiting'
		waiting--;
		up(barbers); // one barber is now ready to cut hair
		up(mutexSem); // release waiting
		cutHair(id); // cut hair
	}
}

static void Customer(int id)
{
	down(mutexSem); // enter critical region
	if (waiting < chairs)
	{
		waiting++;
		up(customers); // wake up barber if necessary
		up(mutexSem); // release access to 'waiting'
		down(barbers); // go to sleep if # of free barbers = 0
		getHaircut(id); // be seated and served
	}
	else
	{
		up(mutexSem); // shop is full, do not wait
		cout << "Not enough chairs, customer " << id << " leaves" << endl;
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
		cout << "Enter number of customers: ";
		cin >> customers;
	}

	while (barbers <= 0)
	{
		cout << "Enter number of barbers: ";
		cin >> barbers;
	}

	// Create threads
	for (int i = 0; i < barbers; i++) {
		//barberThreads.push_back(thread(Barber, i, std::ref(mutexSem)));
		barberThreads.push_back(thread([i, ref(mutexSem)] { Barber(i); }));
	}

	for (int i = 0; i < customers; i++) {
		//clientThreads.push_back(thread(Customer, i, std::ref(mutexSem)));
		clientThreads.push_back(thread([i, ref(mutexSem)] { Customer(i); }));
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

