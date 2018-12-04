#pragma once
#include "Strings.h"

class Value
{
public:
	Value();
	~Value();

	vector <uint8_t> octet;
	vector <char> octetWord;

	void setValue(string pValue);
	void setValue(long long pValue, int pLength);
};

