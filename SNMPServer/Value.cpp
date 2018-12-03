#include "pch.h"
#include "Value.h"
#include "Strings.h"


Value::Value()
{
}


Value::~Value()
{
}

void Value::setValue(string pValue)
{
	vector<char> bytes(pValue.begin(), pValue.end());
	octetWord = bytes;
}

void Value::setValue(long long pValue, unsigned int pLength)
{
	for (int i = pLength-1; i >= 0; i--)
	{
		octet.push_back((pValue >> (8 * i)) & 0xff);
	}
}
