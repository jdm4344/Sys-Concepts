#include "pch.h"
#include "Parser.h"
// Jordan Machalek
// HW 3
// NOTE: Type parsing not implemented

// Constructor
Parser::Parser()
{
	sections = new map <string, map <string, string>>();
	subSections = new map <string, map <string, string>>();
	newestSection = "";
	newestSubsection = "";
}

// Destructor
Parser::~Parser()
{
	delete sections;
	delete subSections;
}

// Prints a list of names of all sections, not including subsections
void Parser::ListAllSections()
{
	cout << "Listing all sections:" << endl;

	for (map<string, map<string,string>>::iterator it = sections->begin(); it != sections->end(); ++it) 
	{
		cout << it->first << endl;
	}
}

// Prints the data within a section/subsection if it exists
void Parser::ListNamedSection(string sectionName)
{
	cout << "Checking if section " << sectionName << "exists..." << endl;

	if (sections->find(sectionName) != sections->end())
	{
		cout << "Section " << sectionName << " exists." << endl;
	}
	else
	{
		cout << "Section " << sectionName << " does not exist." << endl;
	}
}

// Prints the names of all Subsections in a given section
void Parser::ListSubsections(string sectionName)
{
	cout << "Listing subsections of " << sectionName << ":" << endl;

	// Loop through all subsections and check if they belong to the specified sectiion
	for (map<string, map<string, string>>::iterator it = subSections->begin(); it != subSections->end(); ++it)
	{
		// Check current subsection for name of main section
		if (it->first.find(sectionName) != string::npos)
		{
			// Print from the : to end
			int colonPos = it->first.find_first_of(":");
			string sub = it->first.substr(colonPos, it->first.length() - 1);
			cout << sub << endl;
		}
	}
}

// Prints all key-value pairs in a given section/subsection
void Parser::ListAllEntries(string sectionName)
{
	cout << "Listing all entries in " << sectionName << ":" << endl;

	// First, check if the given section is a subsection
	if (regex_match(sectionName, regSubsection, regex_constants::match_any))
	{
		map<string, string> subSection = subSections->find(sectionName)->second;

		for (map<string, string>::iterator it = subSection.begin(); it != subSection.end(); ++it)
		{
			cout << it->first << "=" << it->second << endl;
		}
	}
	else // just a section
	{
		map<string, string> section = sections->find(sectionName)->second;

		for (map<string, string>::iterator it = section.begin(); it != section.end(); ++it)
		{
			cout << it->first << "=" << it->second << endl;
		}
	}
}

// Prints an entry at a given key from a given section/subsection
void Parser::GetEntry(string sectionName, string key)
{
	// First, check if the given section is a subsection
	if (regex_match(sectionName, regSubsection, regex_constants::match_any))
	{
		map<string, string> subSection = subSections->find(sectionName)->second;

		cout << "Entry at " << key << " in " << sectionName << " is " << subSection.find(key)->second << endl;
	}
	else // just a section
	{
		map<string, string> section = sections->find(sectionName)->second;

		cout << "Entry at " << key << " in " << sectionName << " is " << section.find(key)->second << endl;
	}
}

// Prints the key part of all entries in a section
void Parser::GetKey(string sectionName, string key)
{
	cout << "Listing all keys in section: " << sectionName << endl;

	// First, check if the given section is a subsection
	if (regex_match(sectionName, regSubsection, regex_constants::match_any))
	{
		map<string, string> subSection = subSections->find(sectionName)->second;

		for (map<string, string>::iterator it = subSection.begin(); it != subSection.end(); ++it)
		{
			cout << it->first;
		}
	}
	else // just a section
	{
		map<string, string> section = sections->find(sectionName)->second;

		for (map<string, string>::iterator it = section.begin(); it != section.end(); ++it)
		{
			cout << it->first;
		}
	}
}

// Prints the value part of all entries in a section
void Parser::GetValue(string sectionName, string key)
{
	cout << "Listing all values in section: " << sectionName << endl;

	// First, check if the given section is a subsection
	if (regex_match(sectionName, regSubsection, regex_constants::match_any))
	{
		map<string, string> subSection = subSections->find(sectionName)->second;

		for (map<string, string>::iterator it = subSection.begin(); it != subSection.end(); ++it)
		{
			cout << it->second;
		}
	}
	else // just a section
	{
		map<string, string> section = sections->find(sectionName)->second;

		for (map<string, string>::iterator it = section.begin(); it != section.end(); ++it)
		{
			cout << it->second;
		}
	}
}

// Prints the type of an entry
void Parser::GetType(string sectionName, string key)
{
	string dataType = "";

	cout << "The type of " << " is " << dataType << endl;
}

// Parses a vector of strings and converts to sections and associated key-value pairs
void Parser::ParseFile(vector<string> fileContents)
{
	cout << "In parser" << endl;

	/* 
	check for comment
	if comment, check if there are characters before it
	if other characters, get index of first character of the comment and create a substring with only what preceeds the comment
	then, perform other regex checks
	*/

	string currentLine = "";

	for (int i = 0; i < fileContents.size(); i++)
	{
		currentLine = fileContents[i];

		// Remove comments
		if (regex_match(currentLine, regComment, regex_constants::match_any))
		{
			// There is a comment
			int commentPos = currentLine.find_first_of("#"); // Find the start of the comment
			if (commentPos > currentLine.length() || commentPos == 0)
			{
				// # either does not exist or is at the start of the line, so there is no other content
				break;
			}
			else
			{
				// There is more to the line so parse it further
				// Set the line equal to itself minus what is between the comment start and end
				cout << "Line " << i << " contains a comment, removing" << endl;
				fileContents[i] = currentLine.substr(0, currentLine.length() - commentPos);

				RegexSwitch(currentLine, i);
			}
		}
		else
		{
			// There is no comment
			RegexSwitch(currentLine, i);
		}
	}
}

// Helper method containing regex parsing for non-comment lines
void Parser::RegexSwitch(string line, int lineIndex)
{
	string formattedLine = "";

	if (regex_match(line, regSection, regex_constants::match_any)) // line is a section
	{
		// Remove the brackets from the section name
		formattedLine = line.substr(1, line.length() - 2);
		map<string, string> emptyMap; // new empty map
		sections->insert(pair<string, map<string, string>>(formattedLine, emptyMap));
		
		// Save as the latest section that was added
		newestSection = formattedLine;
		// if there is a new section, the last subsection is invalid so reset it:
		newestSubsection = "";
	}
	else if (regex_match(line, regSubsection, regex_constants::match_any)) // line is a subsection
	{
		// Remove the brackets from the section name
		formattedLine = line.substr(1, line.length() - 2);
		map<string, string> emptyMap; // new empty map
		sections->insert(pair<string, map<string, string>>(formattedLine, emptyMap));

		// Save as the latest subsection that was added
		newestSubsection = formattedLine;
	}
	else if (regex_match(line, regKeyValue, regex_constants::match_any)) // line is a key-value pair
	{
		// Separate the key and value
		int equalsPos = line.find_first_of("=");
		string key = line.substr(0, equalsPos - 1);
		string value = line.substr(equalsPos + 1, line.length());

		// if there is a newestSubsection, we are within it, so place the key-value there
		if (newestSubsection != "")
		{
			// subsection -> matching map -> add key-value to that map
			subSections->find(newestSubsection)->second.insert(pair<string, string>(key, value));
		}
		else // we're not in a subsection
		{
			sections->find(newestSection)->second.insert(pair<string, string>(key, value));
		}
	}
	else if (regex_match(line, regKeyValueList, regex_constants::match_any)) // line is a key=list pair
	{
		/// TODO: This is doing the same thing as the prior if() so combine the two
		
		// Separate the key and value
		int equalsPos = line.find_first_of("=");
		string key = line.substr(0, equalsPos - 1);
		string value = line.substr(equalsPos + 1, line.length());

		// if there is a newestSubsection, we are within it, so place the key-value there
		if (newestSubsection != "")
		{
			// subsection -> matching map -> add key-value to that map
			subSections->find(newestSubsection)->second.insert(pair<string, string>(key, value));
		}
		else // we're not in a subsection
		{
			sections->find(newestSection)->second.insert(pair<string, string>(key, value));
		}
	}
	else
	{
		cout << "Error! Line " << lineIndex << " is empty or does not use a supported format." << endl;
	}
}


