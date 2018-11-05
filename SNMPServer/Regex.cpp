#include "pch.h"
#include "Regex.h"


Regex::Regex()
{
}


Regex::~Regex()
{
}

regex Regex::wzorzec()
{
	return regex("\\w*\\s*OBJECT-TYPE\\s*SYNTAX.*ACCESS.*STATUS.*DESCRIPTION\\s*.*\\s*::=\\s*\{.*\}");
}

regex Regex::whitespaces()
{
	return regex("\\s{2,}");
}

regex Regex::comments()
{
	return regex("-{2}.*");
}