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