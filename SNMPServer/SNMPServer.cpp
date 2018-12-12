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

	int encodingType = 0, type = 0, typeID = 0;
	string nodeNameOrOID = "", valueToEncode = "", keyword = "", visibility = "";
	long long byteCount = 0;

	string encodeType;
	vector<int> sequenceDataTypes;
	vector <string> sequenceDataValues;
	int sequenceDataType;
	string sequenceDataValue;

	vector<int> sequenceTypeId;
	vector<string> sequenceKeywords, sequenceVisibility;

	long long sequenceDataSize;
	vector<long long> sequenceDataSizes;

	parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
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

	cout << "-------------------------------------------------------" << endl << endl;
	cout << "Wybierz typ kodowania:" << endl << "1 - kodowanie liscia drzewa" << endl << "2 - kodowanie dowolnej wartosci" << endl;
	cin >> encodingType;

	if (encodingType == 1)
	{
		cout << endl << "Podaj nazwe lub OID liscia:" << endl;
		cin >> nodeNameOrOID;

		TreeNode* node = OIDTree.findOID(nodeNameOrOID, OIDTree.root);
		if (node == OIDTree.root)
		{
			node = OIDTree.findNode(nodeNameOrOID, OIDTree.root);
		}
		if (node != nullptr && node != OIDTree.root)
		{
			cout << "Podaj wartosc do zakodowania:" << endl;
			cin >> valueToEncode;

			int result = checkValue.checkValue(valueToEncode, node, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
			if (result == 0)
			{
				if (checkValue.typeDataType != 0)
				{
					typeID = vDataType.at(checkValue.indexDataType).typeID;
					byteCount = checkValue.byteCount;
					keyword = vDataType.at(checkValue.indexDataType).keyword;
					visibility = vDataType.at(checkValue.indexDataType).visibility;
					type = checkValue.typeDataType;
				}
				else if (checkValue.type != 0)
				{
					typeID = NULL;
					byteCount = checkValue.byteCount;
					keyword = "";
					visibility = "";
					type = checkValue.type;
				}
				else
				{
					typeID = NULL;
					byteCount = NULL;
					keyword = "";
					visibility = "";
					type = SEQUENCE_TAG_NUMBER;
				}

				string test = coder.encode(valueToEncode, type, typeID, byteCount, keyword, visibility, checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities);
				cout << endl << "Zakodowana wartosc:" << endl;
				cout << test << endl;
			}
			else if (result == -1)
			{
				cout << "Blad typu" << endl;
			}
			else if (result == -2)
			{
				cout << "Blad rozmiaru" << endl;
			}
		}
	}
	else if (encodingType == 2)
	{
		cout << "Wybierz typ danych do zakodowania" << endl << "1 - INTEGER" << endl << "2 - OCTET STRING" << endl << "3 - NULL" << endl << "4 - OBJECT IDENTIFIER" << endl << "5 - SEQUENCE" << endl;
		cin >> encodingType;
		if (encodingType != 5)
		{
			cout << "Podaj rozmiar danych w bajtach" << endl;
			cin >> byteCount;
			cout << "Podaj dane do zakodowania" << endl;
			cin >> valueToEncode;

			typeID = NULL;
			keyword = "";
			visibility = "";

			switch (encodingType)
			{
			case 1:
				type = INTEGER_TAG_NUMBER;
			case 2:
				type = OCTET_STRING_TAG_NUMBER;
			case 3:
				type = NULL_TAG_NUMBER;
			case 4:
				type = OBJECT_IDENTIFIER_TAG_NUMBER;
			case 5:
				type = SEQUENCE_TAG_NUMBER;
			default:
				type = 0;
				break;
			}

			string test = coder.encode(valueToEncode, type, typeID, byteCount, keyword, visibility, checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities);
			cout << endl << "Zakodowana wartosc:" << endl;
			cout << test << endl;
		}
		else
		{
			cout << "Podaj rozmiar sekwencji w bajtach" << endl;
			cin >> byteCount;
			cout << "Podaj typy danych do zakodowania" << endl << "2 - INTEGER" << endl << "4 - OCTET STRING" << endl << "5 - NULL" << "6 - OBJECT IDENTIFIER" << endl << "16 - SEQUENCE" << endl;
			for (size_t i = 0; i < byteCount; i++)
			{
				cin >> sequenceDataType;
				sequenceDataTypes.push_back(sequenceDataType);
			}
			cout << "Podaj dane do zakodowania" << endl;
			for (size_t i = 0; i < byteCount; i++)
			{
				cin >> sequenceDataValue;
				sequenceDataValues.push_back(sequenceDataValue);
			}
			cout << "Podaj rozmiar danych do zakodowania" << endl;
			for (size_t i = 0; i < byteCount; i++)
			{
				cin >> sequenceDataSize;
				sequenceDataSizes.push_back(sequenceDataSize);
			}

			typeID = NULL;
			byteCount = NULL;
			keyword = "";
			visibility = "";
			type = SEQUENCE_TAG_NUMBER;

			string test = coder.encode(valueToEncode, type, typeID, byteCount, keyword, visibility, sequenceDataValues, sequenceDataTypes, sequenceTypeId, sequenceDataSizes, sequenceKeywords, sequenceVisibility);
			cout << endl << "Zakodowana wartosc:" << endl;
			cout << test << endl;
		}
	}
	else
	{

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