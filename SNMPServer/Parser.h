#pragma once
#include "TreeStructure.h"
#include "DataStructures.h"

class Parser
{
public:
	Parser();
	~Parser();

	void wholeFileParse(string pFilePath, Tree pOIDTree, vector<DataType> &pVDataType, vector<Sequence> &pVSequence, vector<Choice> &pVChoice);
};

