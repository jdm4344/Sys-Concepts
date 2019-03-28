#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	// Methods
	vector<string> ListAllSections();
	map<string, string> ListNamedSection();
	vector<string> ListSubsections();
	map<string, string> ListAllEntries();
	string GetEntry(string section, string key);
	string GetKey(int index);
	string GetValue(string key);
	string GetType(string key);

	// Variables
	map < string,
		map<
			map<string, string>,
			map<string,
				map<string, string>
				>
			>
		> sections;
	/*	map <section name,
			map < 
				map	<main section key-value pairs>,
				map	<subsection names, 
					map <subsection key-value pairs>,
					>
				>
			>
	*/
private:
	void ParseFile(char* fileContents);
};

