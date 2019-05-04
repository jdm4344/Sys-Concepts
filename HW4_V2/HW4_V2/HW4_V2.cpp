// HW4_V2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

int numChairs = 0;
int numClients = 0;
int numBarbers = 0;
int waiting = 0;

Semaphore customers;
Semaphore barbers;
Semaphore mutexSem;

vector<thread> clientThreads;
vector<thread> barberThreads;

struct BoundedBuffer {
	int* buffer;
	int capacity;

	int front;
	int rear;
	int count;

	std::mutex lock;

	std::condition_variable not_full;
	std::condition_variable not_empty;

	BoundedBuffer(int capacity) : capacity(capacity), front(0), rear(0), count(0) {
		buffer = new int[capacity];
	}

	~BoundedBuffer() {
		delete[] buffer;
	}

	void deposit(int data) {
		std::unique_lock<std::mutex> l(lock);

		not_full.wait(l, [this]() {return count != capacity; });

		buffer[rear] = data;
		rear = (rear + 1) % capacity;
		++count;

		l.unlock();
		not_empty.notify_one();
	}

	int fetch() {
		std::unique_lock<std::mutex> l(lock);

		not_empty.wait(l, [this]() {return count != 0; });

		int result = buffer[front];
		front = (front + 1) % capacity;
		--count;

		l.unlock();
		not_full.notify_one();

		return result;
	}
};

class Semaphore {
public:
	Semaphore(int ct = 0)
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

private:
	mutex mtx;
	condition_variable cv;
	int count;
};

void Barber(int id, Semaphore& sem) {
	while (true)
	{
		customers.down(id); // down(customers); // go to sleep if # of customers = 0
		mutexSem.down(id); // dowm(mutex); // acquire access to 'waiting'
		waiting--;
		barbers.up(id); // up(barbers); // one barber is now ready to cut hair
		mutexSem.up(id); // up(mutex); // release waiting;
		 // cutHair(); // cut hair
	}
}

void Customer(int id, Semaphore& sem)
{
	if (waiting < numChairs)
	{
		// down(mutex);
		waiting++; // increment count of waiting customers
		// up(customers); // wake up barber if necessary
		// up(mutex); // release access to 'waiting'
		// down(barbers); // go to sleep if # of free barbers = 0
		// getHaircut(); // be seated and served
		cout << "Customer " << id << " is recieving their haircut" << endl;
	}
	else
	{
		// up(mutex); // shop is full, do not wait
		cout << "Not enough chairs, customer " << id << " leaves" << endl;
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

	BoundedBuffer chairs(numChairs);

	customers = Semaphore(numClients);
	barbers = Semaphore(numBarbers);
	mutexSem = Semaphore(numChairs);

	// Create threads
	for (int i = 0; i < numBarbers; i++) {
		barberThreads.push_back(thread(Barber, i, std::ref(mutexSem)));
	}

	for (int i = 0; i < numClients; i++) {
		clientThreads.push_back(thread(Customer, i, std::ref(mutexSem)));
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

