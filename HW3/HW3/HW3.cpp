// HW3.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Jordan Machalek
// HW 3
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include "Parser.h"

int main()
{
	Parser regexParser;
	// CHANGE INPUT FILE HERE:
	ifstream inFile("config.txt", ios::binary);
	vector<string> fileContents;

	if (inFile.is_open())
	{
		// Get file length
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(0, ios::beg);
		
		// Save file line by line as strings
		string line;

		while (getline(inFile, line))
		{
			fileContents.push_back(line);
		}

		// List out the base file for clarity
		cout << "File contents:" << endl;
		
		for (size_t i = 0; i < fileContents.size(); i++)
		{
			cout << fileContents[i] << endl;
		}

		// Close file
		inFile.close();
	}

	// Send the file to the parser
	regexParser.ParseFile(fileContents);

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
