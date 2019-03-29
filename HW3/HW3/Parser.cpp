#include "pch.h"
#include "Parser.h"

Parser::Parser()
{

}

Parser::~Parser()
{

}

vector<string> Parser::ListAllSections()
{
	return vector<string>();
}

map<string, string> Parser::ListNamedSection()
{
	return map<string, string>();
}

vector<string> Parser::ListSubsections(string section)
{
	return vector<string>();
}

map<string, string> Parser::ListAllEntries(string section)
{
	return map<string, string>();
}

string Parser::GetEntry(string section, string key)
{
	return string();
}

string Parser::GetKey(string section, string key)
{
	return string();
}

string Parser::GetValue(string section, string key)
{
	return string();
}

string Parser::GetType(string section, string key)
{
	return string();
}

void Parser::ParseFile(char * fileContents)
{
}


