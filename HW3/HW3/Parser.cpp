#include "pch.h"
#include "Parser.h"

// Constructor
Parser::Parser()
{
	sections = new map <string, map <string, string>>();
	subSections = new map <string, map <string, string>>();
}

// Destructor
Parser::~Parser()
{
	delete sections;
	delete subSections;
}

// Returns a list of names of all sections, not including subsections
void Parser::ListAllSections()
{
	cout << "Listing all sections:" << endl;

	for (map<string, map<string,string>>::iterator it = sections->begin(); it != sections->end(); ++it) 
	{
		cout << it->first << endl;
	}
}

// Returns a section by name if it exists
void Parser::ListNamedSection(string sectionName)
{
	cout << "Listing section " << sectionName << ":" << endl;

	if (sections->find(sectionName) != sections->end())
	{
		map<string, string> section = sections->at(sectionName);
		

	}
}

//
void Parser::ListSubsections(string sectionName)
{
	cout << "Listing subsections of " << sectionName << ":" << endl;


}

//
void Parser::ListAllEntries(string sectionName)
{
	cout << "Listing all entries in " << sectionName << ":" << endl;
}

// 
void Parser::GetEntry(string section, string key)
{
	cout << "Value at key " << key << " is: " << endl;
}

//
void Parser::GetKey(string section, string key)
{

}

//
void Parser::GetValue(string section, string key)
{

}

//
void Parser::GetType(string section, string key)
{
	string dataType;

	cout << "The type of " << " is " << dataType << endl;
}

// Parses a vector of strings and converts to sections and associated key-value pairs
void Parser::ParseFile(vector<string> fileContents)
{
	// Define regexes
	regex comment { "(?!.*[\"].*)#\\s*(.)+" };
	regex section { "(?!\\[.*:.*\\])^(?!.*[#{}])^\\[.+\\]" }; // Ignore sections including # // Backup: (?:(?!\\[.*:.*\\])^\\[.+\\])
	regex subsection { "^\\[[a-zA-Z0-9]+:[a-zA-Z0-9]+\\]" }; // ^\\[.+:.+\\]
	regex keyValue { "(?!.*[;:\\[\\]].*)(?!^#)(?![#])(^[a-zA-Z0-9]+=.+)" }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	regex keyValueList { ".+={.+}" };

	/* 
	check for comment
	if comment, check if there are characters before it
	if other characters, get index of first character of the comment and create a substring with only what preceeds the comment
	then, perform other regex checks
	*/

	for (int i = 0; i < fileContents.size(); i++)
	{

	}
}


