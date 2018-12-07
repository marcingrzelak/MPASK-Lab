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

	bool isValueNumber;
	long long pValueINT = LONG_MIN, byteCount, bitCount;
	int type = 0, typeDataType = 0;
	int indexDataType = -1;

	void setIdentifier(string pClass, string pComplexity, string pType);
	void setIdentifier(string pClass, string pComplexity, unsigned int pTag);
	void clearIdentifier();

	bool undefinedFlag = false;
	void setLength(unsigned long long pLength);
	void clearLength();

	void setValue(string pValue);
	void setValue(long long pValue, int pLength);
	void clearValue();

	void checkIsNumber(string pValue);
	void lengthCalc(string pValue);

	string concatAllValues(bool pIsValueNumber);

	int checkValueType(string pValue, TreeNode* pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize);

	short defaultTypeCheck(string pValue, bool &isValueNumber);
	short dataTypeCheck(string pValue, vector<DataType> &pVDataType);

	int checkValueSize(string pValue, TreeNode * pNode, vector<ObjectTypeSize>& pVObjectTypeSize, vector<DataType> &pVDataType);

	short objectTypeSizeCheck(vector<ObjectTypeSize> pVObjectTypeSize, string pName);
	short dataTypeSizeCheck(vector<DataType>& pVDataType);
	short checkSize(int pSize, long long pSizeMin, long long pSizeMax);

	string encode(TreeNode* pNode, string pValue, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize);
};