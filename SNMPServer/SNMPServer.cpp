#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"
#include "Parser.h"

int main()
{
	Tree OIDTree;
	Parser parser;
	string testFile = "MIBS/RFC-1213-MIB.txt";

	parser.wholeFileParse(testFile, OIDTree);

	OIDTree.root->printTree("", true);
	return(0);
}