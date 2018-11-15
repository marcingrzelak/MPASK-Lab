#pragma once
#include "TreeStructure.h"

class Parser
{
public:
	Parser();
	~Parser();

	void wholeFileParse(string pFilePath, Tree pOIDTree);
};

