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

void Value::setValue(long long pValue, int pLength)
{
	if (pLength == 0)
	{
		octet.push_back(0);
	}
	else
	{
		for (int i = pLength - 1; i >= 0; i--)
		{
			octet.push_back((pValue >> (8 * i)) & 0xff);
		}
	}
}

void Value::setValue(vector<string> pObjectIdentifierSubidentifiers)
{
	int X = stoi(pObjectIdentifierSubidentifiers.at(0));
	int Y = stoi(pObjectIdentifierSubidentifiers.at(1));
	int firstSubidentifier = X * 40 + Y;
	valueObjectIdentifier(firstSubidentifier);
	

	for (size_t i = 2; i < pObjectIdentifierSubidentifiers.size(); i++)
	{
		int pOISint = stoi(pObjectIdentifierSubidentifiers.at(i));
		valueObjectIdentifier(pOISint);
	}
}

void Value::valueObjectIdentifier(int pSubidentifier)
{
	if (pSubidentifier > 127)
	{
		uint8_t x = 0, oneOctet = 0;
		int i = 31;//4 bajty
		unsigned int bitCounter = 0;
		bool flag = false;
		while (i >= 0)
		{
			x = (pSubidentifier >> i) & 1; //bierzemy kolejne bity

			if (x != 0 || flag) //wykrycie pierwszego bitu '1'
			{
				bitCounter = i % 7; //ktory bit w 7-bitowym ciagu
				oneOctet |= x << bitCounter;
				flag = true;
				if (bitCounter == 0) //dodawanie 1 bitu (ostatni/nieostatni bajt) co 7 bitow
				{
					if (i != 0)
					{
						oneOctet |= 0x80; //dodanie bitu "nieostatni bajt"
					}
					octet.push_back(oneOctet);
					oneOctet = 0;
				}
			}
			i--;
		}
	}
	else
	{
		uint8_t oneOctet = 0;
		oneOctet |= pSubidentifier;
		octet.push_back(oneOctet);
	}
}
