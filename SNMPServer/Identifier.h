#pragma once
#include "pch.h"
#include "Strings.h"

class Identifier
{
public:
	Identifier();
	~Identifier();

	int8_t firstOctet;
	vector <int8_t> longOctet;

	map <string, int8_t> classes = {
		{IDENTIFIER_CLASS_UNIVERSAL, 0b00000000},
		{IDENTIFIER_CLASS_APPLICATION, 0b01000000},
		{IDENTIFIER_CLASS_CONTEXT_SPECIFIC, 0b10000000},
		{IDENTIFIER_CLASS_PRIVATE, 0b11000000},
	};

	map <string, int8_t> complexity = {
		{IDENTIFIER_COMPLEXITY_PRIMITIVE, 0b00000000},
		{IDENTIFIER_COMPLEXITY_CONSTRUCTED, 0b00100000},
	};

	map <string, int8_t> tags = {
	{IDENTIFIER_TYPE_INTEGER, 0x02},
	{IDENTIFIER_TYPE_OCTET_STRING, 0x04},
	{IDENTIFIER_TYPE_NULL, 0x05},
	{IDENTIFIER_TYPE_OBJECT_IDENTIFIER, 0x06},
	{IDENTIFIER_TYPE_SEQUENCE, 0x10},
	{IDENTIFIER_TYPE_SEQUENCE_OF, 0x10},
	};

	void setClass(string pClass);
	void setComplexity(string pComplexity);
	void setTag(unsigned int pTag);
	void setTagFromType(string pType);
};

