#pragma once
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "TreeStructure.h"
#include "DataStructures.h"

class BERCoder
{
public:
	BERCoder();
	~BERCoder();

	Identifier identifier;
	Length length;
	Value value;

	void setIdentifier(string pClass, string pComplexity, string pType);
	void setIdentifier(string pClass, string pComplexity, unsigned int pTag);
	void clearIdentifier();

	bool undefinedFlag = false;
	void setLength(unsigned long long pLength);
	void clearLength();

	void setValue(string pValue);
	void setValue(long long pValue, int pLength);
	void clearValue();

	string concatAllValues(bool pIsValueNumber);
	string encode(string pValue);
};