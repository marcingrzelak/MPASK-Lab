#pragma once
#include "Identifier.h"
class BERCoder
{
public:
	BERCoder();
	~BERCoder();

	Identifier identifier;

	void setIdentifier(string pClass, string pComplexity, string pType);
	void setIdentifier(string pClass, string pComplexity, unsigned int pTag);
};

