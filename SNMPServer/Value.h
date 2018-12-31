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
	void setValue(vector<string> pObjectIdentifierSubidentifiers);
	void valueObjectIdentifier(int pSubidentifier);

	vector<uint8_t> getValue(vector<uint8_t> pOctets, int &index, unsigned long long pLength);
};

