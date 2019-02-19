#pragma once
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Exceptions.h"
#include "Strings.h"

class Parser
{
public:
	Parser();
	~Parser();

	void wholeFileParse(string pFilePath, Tree pOIDTree, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize);
};

