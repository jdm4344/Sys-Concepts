#pragma once
// Jordan Machalek
// HW 3

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
	void GetEntry(string sectionName, string key);
	void GetKey(string sectionName, string key);
	void GetValue(string sectionName);
	void GetType(string sectionName, string key);
	void ParseFile(vector<string> fileContents);
	void RegexSwitch(string line, int lineIndex); // Helper method containing regex parsing for non-comment lines

	// Variables
	map <string, map <string, string>>* sections; // map <sectionName, map <key, value>>
	map <string, map <string, string>>* subSections; // map <subsectionName, map <key, value>>
	string newestSection;
	string newestSubsection;
private:
	// Define regex patterns
	regex regComment{ "(?!.*[\\\"].*)#\\s*.+" };
	regex regSection{ "(?!\\[.*:.*\\])^(?!.*[#{}])^\\[.+\\]" };  // Backup: (?:(?!\\[.*:.*\\])^\\[.+\\])
	regex regSubsection{ "^\\[[a-zA-Z0-9]+:[a-zA-Z0-9]+\\]" }; // ^\\[.+:.+\\]
	regex regKeyValue{ "(?!.*[;:\\[\\]].*)(?!^#)(?![#])(^[a-zA-Z0-9]+=.+)" }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	regex regKeyValueList{ "(?!#.*).+=\\{.+\\}" }; //.+=\\{.+\\}
	regex regBoolType{ "true|TRUE|false|FALSE" };
	regex regFloatType{ "[0-9]+\.[0-9]+f" };
	regex regIntType{ "(?!\.)[0-9]+" };
	regex regStringType{ "\\\".*\\\"" };
	regex regListType{ "\\{.+\\}" };

	//regex regComment { "." };
	//regex regSection { "." };  // Backup: (?:(?!\\[.*:.*\\])^\\[.+\\])
	//regex regSubsection { "." }; // ^\\[.+:.+\\]
	//regex regKeyValue { "." }; // #; exlcudes comments and list values // (?!.*[#;:].*)^.+=.+
	//regex regKeyValueList { "." };
};

