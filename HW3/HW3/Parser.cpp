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

// Prints a list of names of all sections, not including subsections
void Parser::ListAllSections()
{
	cout << "Listing all sections:" << endl;

	for (map<string, map<string,string>>::iterator it = sections->begin(); it != sections->end(); ++it) 
	{
		cout << it->first << endl;
	}
}

// Prints a section by name if it exists
void Parser::ListNamedSection(string sectionName)
{
	cout << "Listing section " << sectionName << ":" << endl;

	if (sections->find(sectionName) != sections->end())
	{
		map<string, string> section = sections->at(sectionName);
		

	}
}

// Prints all Subsections in a given section
void Parser::ListSubsections(string sectionName)
{
	cout << "Listing subsections of " << sectionName << ":" << endl;


}

// Prints all entries in a given section
void Parser::ListAllEntries(string sectionName)
{
	cout << "Listing all entries in " << sectionName << ":" << endl;
}

// Prints an entry by key from a given section/subsection
void Parser::GetEntry(string section, string key)
{
	cout << "Value at key " << key << " is: " << endl;
}

// Prints the key of an entry
void Parser::GetKey(string section, string key)
{

}

// Prints the value of an entry
void Parser::GetValue(string section, string key)
{

}

// Prints the type of an entry
void Parser::GetType(string section, string key)
{
	string dataType;

	cout << "The type of " << " is " << dataType << endl;
}

// Parses a vector of strings and converts to sections and associated key-value pairs
void Parser::ParseFile(vector<string> fileContents)
{
	// Define regexes
	regex regComment { "(?!.*[\"].*)#\\s*(.)+" };
	regex regSection { "(?!\\[.*:.*\\])^(?!.*[#{}])^\\[.+\\]" }; // Ignore sections including # // Backup: (?:(?!\\[.*:.*\\])^\\[.+\\])
	regex regSubsection { "^\\[[a-zA-Z0-9]+:[a-zA-Z0-9]+\\]" }; // ^\\[.+:.+\\]
	regex regKeyValue { "(?!.*[;:\\[\\]].*)(?!^#)(?![#])(^[a-zA-Z0-9]+=.+)" }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	regex regKeyValueList { ".+={.+}" };

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
			int commentPosit = currentLine.find_first_of("#"); // Find the start of the comment
			if (commentPosit > currentLine.length() || commentPosit == 0)
			{
				// # either does not exist or is at the start of the line, so there is no other content
				break;
			}
			else
			{
				// There is more to the line so parse it further
				// Set the line equal to itself minus what is between the comment start and end
				fileContents[i] = currentLine.substr(0, currentLine.length() - commentPosit);


			}
		}
		else
		{
			// There is no comment
		}
	}
}

// Helper method containing regex parsing for non-comment lines
void Parser::RegexSwitch(string line)
{
	regex regSection{ "(?!\\[.*:.*\\])^(?!.*[#{}])^\\[.+\\]" }; // Ignore sections including # // Backup: (?:(?!\\[.*:.*\\])^\\[.+\\])
	regex regSubsection{ "^\\[[a-zA-Z0-9]+:[a-zA-Z0-9]+\\]" }; // ^\\[.+:.+\\]
	regex regKeyValue{ "(?!.*[;:\\[\\]].*)(?!^#)(?![#])(^[a-zA-Z0-9]+=.+)" }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	regex regKeyValueList{ ".+={.+}" };

	if (regex_match(line, regSection, regex_constants::match_any))
	{

	}
}


