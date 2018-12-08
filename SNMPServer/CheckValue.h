#pragma once
#include "TreeStructure.h"
#include "DataStructures.h"

class CheckValue
{
public:
	CheckValue();
	~CheckValue();

	bool isValueNumber;
	long long pValueINT = LONG_MIN, byteCount, bitCount;
	int type = 0, typeDataType = 0, typeSequence = 0;
	int indexDataType = -1, indexSequence = -1;
	vector<string> sequenceValues;

	void setValueParameters(string pValue);

	void checkIsNumber(string pValue);
	void lengthCalc(string pValue);

	int checkValueType(string pValue, string pSyntax, vector<DataType> &pVDataType, vector<Sequence> &pVSequence);

	short defaultTypeCheck(string pSyntax, bool &isValueNumber);
	short dataTypeCheck(string pValue, string pSyntax, vector<DataType> &pVDataType);
	short sequenceTypeCheck(string pSyntax, vector<Sequence> &pVSequence, vector<DataType> &pVDataType, string pValue);

	int checkValueSize(string pName, vector<ObjectTypeSize>& pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence);

	short objectTypeSizeCheck(vector<ObjectTypeSize> pVObjectTypeSize, string pName);
	short dataTypeSizeCheck(vector<DataType>& pVDataType);
	short sequenceSizeCheck(vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence);
	short checkSize(int pSize, long long pSizeMin, long long pSizeMax);

	short checkValue(string pValue, TreeNode *pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize);
};

