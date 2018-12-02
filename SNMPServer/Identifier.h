#pragma once
#include "pch.h"
#include "Strings.h"

class Identifier
{
public:
	Identifier();
	~Identifier();

	 uint8_t firstOctet;
	 vector <uint8_t> longOctet;

	 map <string, uint8_t> classes;
	 map <string, uint8_t> complexity;
	 map <string, uint8_t> tags;

	 void setClass(string pClass);
	 void setComplexity(string pComplexity);
	 void setTag(unsigned long pTag);
	 void setTagFromType(string pType);
};

