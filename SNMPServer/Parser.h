#pragma once
#include "TreeStructure.h"
#include "DataStructures.h"

class Parser
{
public:
	Parser();
	~Parser();

	void wholeFileParse(string pFilePath, Tree pOIDTree, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize);
};

