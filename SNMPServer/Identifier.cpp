#include "pch.h"
#include "Identifier.h"
#include "Strings.h"


Identifier::Identifier()
{

}


Identifier::~Identifier()
{
}

void Identifier::setClass(string pClass)
{
	if (pClass == IDENTIFIER_CLASS_UNIVERSAL)
	{
		firstOctet | classes[IDENTIFIER_CLASS_UNIVERSAL];
	}
	else if (pClass == IDENTIFIER_CLASS_APPLICATION)
	{
		firstOctet | classes[IDENTIFIER_CLASS_APPLICATION];
	}
	else if (pClass == IDENTIFIER_CLASS_CONTEXT_SPECIFIC)
	{
		firstOctet | classes[IDENTIFIER_CLASS_CONTEXT_SPECIFIC];
	}
	else if (pClass == IDENTIFIER_CLASS_PRIVATE)
	{
		firstOctet | classes[IDENTIFIER_CLASS_PRIVATE];
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
		firstOctet | complexity[IDENTIFIER_COMPLEXITY_PRIMITIVE];
	}
	else if (pComplexity == IDENTIFIER_COMPLEXITY_CONSTRUCTED)
	{
		firstOctet | complexity[IDENTIFIER_COMPLEXITY_CONSTRUCTED];
	}
	else
	{
		//error
	}
}

void Identifier::setTag(unsigned int pTag)
{
	if (pTag <= 31) //one octet
	{
		firstOctet | pTag;

	}
	else //long octet
	{

	}
}

void Identifier::setTagFromType(string pType)
{
	if (pType == IDENTIFIER_TYPE_INTEGER)
	{
		firstOctet | tags[IDENTIFIER_TYPE_INTEGER];
	}
	else if (pType == IDENTIFIER_TYPE_OCTET_STRING)
	{
		firstOctet | tags[IDENTIFIER_TYPE_OCTET_STRING];
	}
	else if (pType == IDENTIFIER_TYPE_NULL)
	{
		firstOctet | tags[IDENTIFIER_TYPE_NULL];
	}
	else if (pType == IDENTIFIER_TYPE_OBJECT_IDENTIFIER)
	{
		firstOctet | tags[IDENTIFIER_TYPE_OBJECT_IDENTIFIER];
	}
	else if (pType == IDENTIFIER_TYPE_SEQUENCE)
	{
		firstOctet | tags[IDENTIFIER_TYPE_SEQUENCE];
	}
	else if (pType == IDENTIFIER_TYPE_SEQUENCE_OF)
	{
		firstOctet | tags[IDENTIFIER_TYPE_SEQUENCE_OF];
	}
	else
	{
		//error
	}
}
