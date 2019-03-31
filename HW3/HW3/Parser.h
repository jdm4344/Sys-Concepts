#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	// Methods
	vector<string> ListAllSections();
	bool ListNamedSection(string sectionName, map <string, string>& section);
	bool ListSubsections(string sectionName, vector<string>& subSection);
	map<string, string> ListAllEntries(string sectionName);
	string GetEntry(string section, string key);
	string GetKey(string section, string key);
	string GetValue(string section, string key);
	string GetType(string section, string key);
	void ParseFile(vector<string> fileContents);

	// Variables
	map <string, map <string, string>>* sections;
	map <string, map <string, string>>* subSections;	
};

