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
	return regex(R"(\s{2,})");
}

regex Regex::comments()
{
	return regex(R"(-{2}.*)");
}

regex Regex::OBJECT_TYPE()
{
	return regex(R"((\w*)\sOBJECT-TYPE\sSYNTAX\s(.*?)ACCESS\s(.*?)STATUS\s(.*?)DESCRIPTION\s(.*?)::=\s\{\s(\S*)\s(\d+)\s\})");
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
	return regex(R"((\w*)\s::=\s\[(\S*?)\s(\d+)\]\s(\S*?)\s(.*?)\s\((((SIZE)\s\((\d+)\))|((\d+)\.{2}(\d+)))\))");
}

regex Regex::OBJECT_IDENTIFIER()
{
	return regex(R"(\s(\S*)\sOBJECT\s*IDENTIFIER\s::=\s\{\s(((\S*)\s(\S*)\((\d+)\)\s(\S*)\((\d+)\))|(\S*))\s(\d+)\s\})");
}