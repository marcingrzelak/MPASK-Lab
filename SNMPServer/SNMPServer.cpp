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
	vector <DataType> vDataType;
	vector <Sequence> vSequence;

	parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vSequence);
	DataType::showDataTypes(vDataType);
	Sequence::showSequences(vSequence);

	cout << "Drzewo OID" << endl;
	OIDTree.root->printTree("", true);
	
	return(0);
}