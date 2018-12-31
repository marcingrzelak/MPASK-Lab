#pragma once
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "CheckValue.h"

class BERDecoder
{
public:
	BERDecoder();
	~BERDecoder();

	Identifier identifier;
	Length length;
	Value value;

	string classValue, complexityValue, dataValue;
	unsigned int tagValue;
	unsigned long long lengthValue;
	bool undefinedFlag = false;

	vector<uint8_t> octets;
	vector<uint8_t> values;

	void getVectorOfBytes(string pValue);
	void getIdentifier(int &index);
	void getLength(int &index);
	void getValue(int &index);

	void decode(string pValue, int & index, TreeBER & pTree);
};