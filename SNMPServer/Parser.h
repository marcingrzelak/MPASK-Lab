#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	void wholeFileParse(string pFilePath, Tree pOIDTree);
};

