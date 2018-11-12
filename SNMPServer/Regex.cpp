#include "pch.h"
#include "Regex.h"


Regex::Regex()
{
}

Regex::~Regex()
{
}

regex Regex::whitespaces()
{
	return regex("\\s{2,}");
}

regex Regex::comments()
{
	return regex("-{2}.*");
}

regex Regex::OBJECT_TYPE()
{
	return regex(R"((\w*)\s*OBJECT-TYPE\s*SYNTAX\s*(.*?)ACCESS\s*(.*?)STATUS\s*(.*?)DESCRIPTION\s*(.*?)::=\s*.\s*(\w*)\s*(\d)\s*.)");
}

regex Regex::IMPORTS1()
{
	return regex(R"(BEGIN\s*IMPORTS\s*(.*?);)");
}

regex Regex::IMPORTS2()
{
	return regex(R"(\s*(.*?)\s*FROM\s*(\S*))");
}

regex Regex::IMPORTS3()
{
	return regex(R"(\s*(.*?),)");
}

regex Regex::DATA_TYPE()
{
	return regex("R((\w*)\s*::=\s*.(UNIVERSAL|APPLICATION|CONTEXT-SPECIFIC|PRIVATE)\s*(\d*).\s*(IMPLICIT|EXPLICIT)\s*(INTEGER|OCTET STRING|OBJECT IDENTIFIER|NULL)\s*.(((SIZE)\s*.(\d*).)|((\d*).{2}(\d*))).)");
}

regex Regex::OBJECT_IDENTIFIER()
{
	return regex(R"(\s*(\S*)\s*OBJECT\s*IDENTIFIER\s*::=\s*.\s*(((\S*)\s*(\S*)\((\d*)\)\s*(\S*)\((\d*)\))|(\S*))\s*(\d*)\s*.)");
}