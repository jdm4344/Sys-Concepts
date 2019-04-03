#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	// Methods
	void ListAllSections();
	void ListNamedSection(string sectionName);
	void ListSubsections(string sectionName);
	void ListAllEntries(string sectionName);
	void GetEntry(string section, string key);
	void GetKey(string section, string key);
	void GetValue(string section, string key);
	void GetType(string section, string key);
	void ParseFile(vector<string> fileContents);

	// Variables
	map <string, map <string, string>>* sections; // map <sectionName, map <key, value>>
	map <string, map <string, string>>* subSections; // map <subsectionName, map <key, value>>
};

