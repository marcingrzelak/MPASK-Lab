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
	vector <Index> vIndex;
	vector <Choice> vChoice;
	vector <Sequence> vSequence;

	parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence);
	DataType::showDataTypes(vDataType);
	Index::showIndexes(vIndex);
	Choice::showChoices(vChoice);
	Sequence::showSequences(vSequence);

	cout << "Drzewo OID" << endl;
	OIDTree.root->printTree("", true);

	//szukanie po OID
	TreeNode * node = OIDTree.findOID("1.3.6.1.2.1.8.5", OIDTree.root);
	cout << node->name << endl;

	//szukanie po nazwie
	string oid = OIDTree.findNodeWord("egpNeighTable", OIDTree.root, "");
	if (oid.size() == 0)
	{
		cout << "Brak wezla o podanej nazwie" << endl;
	}
	else
	{
		cout << "OID wezla o podanej nazwie: " << oid << endl;
	}

	return(0);
}