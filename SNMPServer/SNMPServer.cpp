#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"
#include "Parser.h"
#include "Strings.h"
#include "BERCoder.h"
#include "CheckValue.h"

int main()
{
	Tree OIDTree;
	Parser parser;
	BERCoder coder;
	CheckValue checkValue;

	vector <DataType> vDataType;
	vector <Index> vIndex;
	vector <Choice> vChoice;
	vector <Sequence> vSequence;
	vector <ObjectTypeSize> vObjectTypeSize;

	parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
	//DataType::showDataTypes(vDataType);
	//Index::showIndexes(vIndex);
	//Choice::showChoices(vChoice);
	//Sequence::showSequences(vSequence);

	//cout << "Drzewo OID" << endl;
	//OIDTree.root->printTree("", true);

	//szukanie po OID
	//TreeNode * node = OIDTree.findOID("1.3.6.1.2.1.8.5", OIDTree.root);
	//cout << node->name << endl;

	////szukanie po nazwie
	//string oid = OIDTree.findNodeWord("sysUpTime", OIDTree.root, "");
	//if (oid.size() == 0)
	//{
	//	cout << "Brak wezla o podanej nazwie" << endl;
	//}
	//else
	//{
	//	cout << "OID wezla o podanej nazwie: " << oid << endl;
	//}

	TreeNode* node = OIDTree.findNode("sysUpTime", OIDTree.root);

	//coder.setIdentifier(IDENTIFIER_CLASS_PRIVATE, IDENTIFIER_COMPLEXITY_CONSTRUCTED, IDENTIFIER_TYPE_SEQUENCE);
	//coder.setIdentifier(IDENTIFIER_CLASS_PRIVATE, IDENTIFIER_COMPLEXITY_CONSTRUCTED, 1024);
	//coder.setLength(100000);

	int result = checkValue.checkValue("17", node, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);

	//string test = coder.encode("");
	//cout << endl << endl << "----------------------------------------" << endl << test << endl << "----------------------------------------" << endl;
	return(0);
}