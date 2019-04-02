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
vector<string> Parser::ListAllSections()
{
	vector<string> sectionList;

	for (map<string, map<string,string>>::iterator it = sections->begin(); it != sections->end(); ++it) 
	{
		sectionList.push_back(it->first);
	}

	return sectionList;
}

// Returns a section by name if it exists via parameter
bool Parser::ListNamedSection(string sectionName, map <string, string>& section)
{
	if (sections->find(sectionName) != sections->end())
	{
		section = sections->at(sectionName);
		
		return true;
	}

	return false;
}

//
bool Parser::ListSubsections(string sectionName, vector<string>& subSection)
{
	

	return false;
}

//
map<string, string> Parser::ListAllEntries(string sectionName)
{
	return map<string, string>();
}

//
string Parser::GetEntry(string section, string key)
{
	return string();
}

//
string Parser::GetKey(string section, string key)
{
	return string();
}

//
string Parser::GetValue(string section, string key)
{
	return string();
}

//
string Parser::GetType(string section, string key)
{
	string dataType;

	return dataType;
}

// Parses a vector of strings and converts to sections and associated key-value pairs
void Parser::ParseFile(vector<string> fileContents)
{
	// Define regexes
	regex comment { "(?!.*[\"\\[\\]].*)#\\s*(.)+" }; // ignore section names and values containing '#'
	regex section { "(?:(?!\\[.*:.*\\])^\\[.+\\])" };
	regex subsection { "^\\[[a-zA-Z0-9]+:[a-zA-Z0-9]+\\]" }; // ^\\[.+:.+\\]
	regex keyValue { "(?!.*[;:].*)(?!^#)(?![#])(^[a-zA-Z0-9]+=.+)" }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	regex keyValueList { ".+={.+}" };


}


