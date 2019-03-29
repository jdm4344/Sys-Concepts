#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	// Methods
	vector<string> ListAllSections();
	map<string, string> ListNamedSection();
	vector<string> ListSubsections(string section);
	map<string, string> ListAllEntries(string section);
	string GetEntry(string section, string key);
	string GetKey(string section, string key);
	string GetValue(string section, string key);
	string GetType(string section, string key);

	// Variables
	map < string,
		map<
			map<string, string>,
			map<string,
				map<string, string>
				>
			>
		> 
	sections;
	/*	map <section name,
			map < 
				map	<main section key-value pairs>,
				map	<subsection names, 
					map <subsection key-value pairs>,
					>
				>
			>
	*/
	// MAP <1section name, MAP <2 MAP <3main section key - value pairs>3, MAP <subsection names,MAP <subsection key - value pairs>>>2>1
private:
	void ParseFile(char* fileContents);
};

