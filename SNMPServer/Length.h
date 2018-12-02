#pragma once
#include "pch.h"
#include "Strings.h"

class Length
{
public:
	Length();
	~Length();

	uint8_t firstOctet;
	vector <uint8_t> longOctet;

	int K, bits;

	void setDefinedForm(unsigned long long pLength);
	void setUndefinedForm(unsigned long long pLength);
};

