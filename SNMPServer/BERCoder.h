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

	stringstream concatAllValues(bool pIsValueNumber);
	int checkValue(string pValue, TreeNode* pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<SpecialDataType> &pVSpecialDataType);
	string encode(TreeNode* pNode, string pValue, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<SpecialDataType> &pVSpecialDataType);

	bool isValueNumber;
	long long pValueINT = LONG_MIN;
	long long byteCount, bitCount;
	int type = 0;
};

