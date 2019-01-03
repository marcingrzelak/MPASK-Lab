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

	int encodingType = 0, type = 0, typeID = 0, encodingDataType = 0;
	string nodeNameOrOID = "", valueToEncode = "", keyword = "", visibility = "";
	unsigned long long byteCount = 0;

	string encodeType;
	vector<int> sequenceDataTypes;
	vector <string> sequenceDataValues;
	int sequenceDataType;
	string sequenceDataValue;

	vector<int> sequenceTypeId;
	vector<string> sequenceKeywords, sequenceVisibility;

	unsigned long long sequenceDataSize;
	vector<unsigned long long> sequenceDataSizes;

	//parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
	//DataType::showDataTypes(vDataType);
	//Index::showIndexes(vIndex);
	//Choice::showChoices(vChoice);
	//Sequence::showSequences(vSequence);
	//
	//cout << "Drzewo OID" << endl;
	//OIDTree.root->printTree("", true);
	//
	//szukanie po OID
	//TreeNode * node = OIDTree.findOID("1.3.6.1.2.1.8.5", OIDTree.root);
	//cout << node->name << endl;
	//



	while (true)
	{
		cout << "-------------------------------------------------------" << endl << endl;
		cout << "Wybierz typ kodowania:" << endl << "1 - kodowanie liscia drzewa" << endl << "2 - kodowanie dowolnej wartosci" << endl;
		cin >> encodingType;

		if (encodingType == 1)
		{
			string temp = coder.treeNodeEncoding(OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
			cout << endl << "Zakodowana wartosc:" << endl;
			cout << temp << endl;
		}
		else if (encodingType == 2)
		{
			string temp1 = "", temp = coder.anyValueEncoding(temp1, false);
			cout << endl << "Zakodowana wartosc:" << endl;
			cout << temp << endl;			
		}
		else
		{

		}
	}

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

	//podawanie wartosci sekwencji w formacie {w1,w2,w3,...,wn}
	//podawanie wartosci object identifier w formacie {1.2.3.4.5}


	//cout << endl << endl << "----------------------------------------" << endl << test << endl << "----------------------------------------" << endl;
	return(0);
}