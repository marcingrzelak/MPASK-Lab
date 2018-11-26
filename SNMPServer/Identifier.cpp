#include "pch.h"
#include "Identifier.h"
#include "Strings.h"

Identifier::Identifier()
{
	firstOctet = 0;

	classes[IDENTIFIER_CLASS_UNIVERSAL] = 0b00000000;
	classes[IDENTIFIER_CLASS_APPLICATION] = 0b01000000;
	classes[IDENTIFIER_CLASS_CONTEXT_SPECIFIC] = 0b10000000;
	classes[IDENTIFIER_CLASS_PRIVATE] = 0b11000000;

	complexity[IDENTIFIER_COMPLEXITY_PRIMITIVE] = 0b00000000;
	complexity[IDENTIFIER_COMPLEXITY_CONSTRUCTED] = 0b00100000;

	tags[IDENTIFIER_TYPE_INTEGER] = 0x02;
	tags[IDENTIFIER_TYPE_OCTET_STRING] = 0x04;
	tags[IDENTIFIER_TYPE_NULL] = 0x05;
	tags[IDENTIFIER_TYPE_OBJECT_IDENTIFIER] = 0x06;
	tags[IDENTIFIER_TYPE_SEQUENCE] = 0x10;
	tags[IDENTIFIER_TYPE_SEQUENCE_OF] = 0x10;
}


Identifier::~Identifier()
{
}

void Identifier::setClass(string pClass)
{
	if (pClass == IDENTIFIER_CLASS_UNIVERSAL)
	{
		firstOctet = firstOctet | classes[IDENTIFIER_CLASS_UNIVERSAL];
	}
	else if (pClass == IDENTIFIER_CLASS_APPLICATION)
	{
		firstOctet = firstOctet | classes[IDENTIFIER_CLASS_APPLICATION];
	}
	else if (pClass == IDENTIFIER_CLASS_CONTEXT_SPECIFIC)
	{
		firstOctet = firstOctet | classes[IDENTIFIER_CLASS_CONTEXT_SPECIFIC];
	}
	else if (pClass == IDENTIFIER_CLASS_PRIVATE)
	{
		firstOctet = firstOctet | classes[IDENTIFIER_CLASS_PRIVATE];
	}
	else
	{
		//error
	}
}

void Identifier::setComplexity(string pComplexity)
{
	if (pComplexity == IDENTIFIER_COMPLEXITY_PRIMITIVE)
	{
		firstOctet = firstOctet | complexity[IDENTIFIER_COMPLEXITY_PRIMITIVE];
	}
	else if (pComplexity == IDENTIFIER_COMPLEXITY_CONSTRUCTED)
	{
		firstOctet = firstOctet | complexity[IDENTIFIER_COMPLEXITY_CONSTRUCTED];
	}
	else
	{
		//error
	}
}

void Identifier::setTag(unsigned int pTag)
{
	if (pTag < 31) //one octet
	{
		firstOctet |= pTag;
	}
	else if (pTag > 31) //long octet
	{
		firstOctet |= IDENTIFIER_TAG_LONG;
		uint8_t x = 0, octet = 0;
		unsigned int i = 31, bitCounter = 0;
		bool flag = false;
		while (i >= 0)
		{
			x = (pTag >> i) & 1; //bierzemy kolejne bity

			if (x != 0 || flag) //wykrycie pierwszego bitu '1'
			{
				bitCounter = i % 7; //ktory bit w 7-bitowym ciagu
				octet |= x << bitCounter;
				flag = true;
				if (bitCounter == 0) //dodawanie 1 bitu (ostatni/nieostatni bajt) co 7 bitow
				{
					if (i != 0)
					{
						octet |= 0x80; //dodanie bitu "nieostatni bajt"
					}
					longOctet.push_back(octet);
					octet = 0;
				}
			}
			i--;
		}
	}
	else
	{
		//error
	}
}

void Identifier::setTagFromType(string pType)
{
	if (pType == IDENTIFIER_TYPE_INTEGER)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_INTEGER];
	}
	else if (pType == IDENTIFIER_TYPE_OCTET_STRING)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_OCTET_STRING];
	}
	else if (pType == IDENTIFIER_TYPE_NULL)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_NULL];
	}
	else if (pType == IDENTIFIER_TYPE_OBJECT_IDENTIFIER)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_OBJECT_IDENTIFIER];
	}
	else if (pType == IDENTIFIER_TYPE_SEQUENCE)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_SEQUENCE];
	}
	else if (pType == IDENTIFIER_TYPE_SEQUENCE_OF)
	{
		firstOctet = firstOctet | tags[IDENTIFIER_TYPE_SEQUENCE_OF];
	}
	else
	{
		//error
	}
}
