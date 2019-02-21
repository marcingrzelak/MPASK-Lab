#pragma once
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "CheckValue.h"

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
	unsigned long long setValue(vector<string> pObjectIdentifierSubidentifiers);
	void clearValue();

	string concatAllValues(bool pIsValueNumber);
	string nullEncode();

	string encode(string pValue, int pType, int pTypeID, unsigned long long pByteCount, string pKeyword, string pVisibility, vector<string> pSequenceValues, vector<int> pSequenceTypes, vector<int> pSequenceTypeID, vector<unsigned long long> pSequenceBytesCount, vector<string> pSequenceKeywords, vector<string> pSequenceVisibilities);

	string treeNodeEncoding(string nodeNameOrOID, string valueToEncode, Tree & pOIDTree, vector<DataType>& pVDataType, vector<Index>& pVIndex, vector<Choice>& pVChoice, vector<Sequence>& pVSequence, vector<ObjectTypeSize>& pVObjectTypeSize);
	string anyValueEncoding(string encodedValue, bool isSequence);
};