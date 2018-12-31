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

	string classValue, classConstructedValue, complexityValue, dataValue;
	unsigned int tagValue, tagConstructedValue;
	unsigned long long lengthValue;
	bool undefinedFlag = false;
	TreeNodeBER *currentParent = nullptr;

	vector<uint8_t> octets;
	vector<uint8_t> values;

	void getVectorOfBytes(string pValue);
	void getIdentifier(int &pIndex);
	void getLength(int &pIndex);
	void getValue(int &pIndex);

	void decode(string & pValue, int pIndex, TreeBER & pTree, TreeNodeBER *pParentNode);
};