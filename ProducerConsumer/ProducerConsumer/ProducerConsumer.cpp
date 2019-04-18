// ProducerConsumer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>
using namespace std;

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

void consumer(int id, BoundedBuffer& buffer) {
	for (int i = 0; i < 50; ++i) {
		int value = buffer.fetch();
		std::cout << "Consumer " << id << " fetched " << value << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void producer(int id, BoundedBuffer& buffer) {
	for (int i = 0; i < 75; ++i) {
		buffer.deposit(i);
		std::cout << "Produced " << id << " produced " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	int numProd = 0;
	int numCon = 0;
	int bufferSize = 0;

	vector<thread> producerThreads;
	vector<thread> consumerThreads;

	// Get variables
	while (numProd <= 0) {
		cout << "Enter number of producers: ";
		cin >> numProd;
	}

	while (numCon <= 0) {
		cout << "\nEnter number of consumers: ";
		cin >> numCon;
	}

	//int actorTotal = numProd + numCon;

	while (bufferSize <= 0) {
	//while (bufferSize <= actorTotal) {
		//cout << "\nEnter a buffer size >= number of producers and consumers (" << actorTotal << "): ";
		cout << "\nEnter a buffer size: ";
		cin >> bufferSize;
	}

	// Create buffer
	BoundedBuffer buffer(bufferSize);

	// Create threads
	for (int i = 0; i < numProd; i++) {
		producerThreads.push_back(thread(producer, i, std::ref(buffer)));
	}

	for (int i = 0; i < numProd; i++) {
		consumerThreads.push_back(thread(consumer, i, std::ref(buffer)));
	}

	// Start threads
	for (auto& thread : producerThreads) {
		thread.join();
	}

	for (auto& thread : consumerThreads) {
		thread.join();
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
