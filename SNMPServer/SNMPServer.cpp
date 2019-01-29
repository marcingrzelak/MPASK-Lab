#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"
#include "Parser.h"
#include "Strings.h"
#include "BERCoder.h"
#include "BERDecoder.h"
#include "CheckValue.h"
#include "PDUPackage.h"
#include "Exceptions.h"

int main()
{
	Tree OIDTree;
	TreeBER BERTree;
	Parser parser;
	BERCoder coder;
	BERDecoder decoder;
	CheckValue checkValue;
	PDUPackage pdu;

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

	map<string, string> varBindList;

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




	//cout << "-------------------------------------------------------" << endl << endl;
	//cout << "Wybierz typ kodowania:" << endl << "1 - kodowanie liscia drzewa" << endl << "2 - kodowanie dowolnej wartosci" << endl;
	//cin >> encodingType;

	//string temp, temp1;

	//if (encodingType == 1)
	//{
	//	temp = coder.treeNodeEncoding(OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
	//	cout << endl << "Zakodowana wartosc:" << endl;
	//	cout << temp << endl;
	//}
	//else if (encodingType == 2)
	//{
		/*temp1 = "", temp = coder.anyValueEncoding(temp1, false);
		cout << endl << "Zakodowana wartosc:" << endl;
		cout << temp << endl;*/
	//}
	//else
	//{

	//}


	//int index = 0;
	//decoder.decode(temp, index, BERTree, nullptr);

	//cout << endl << "Odkodowana wartosc:" << endl;
	//BERTree.root->printTree("", true);

	//cout << endl;

/*	varBindList.insert(pair<string, string>("1.3.6.1.2.1.1.1", "05 00"));
	int requestID = 1234, errorStatus = 0, errorIndex = 0;

	cout << endl << endl << "Drzewo PDU" << endl;

	pdu.analyzePacket("30 2c 02 01 00 04 07 70 72 69 76 61 74 65 a0 1e 02 01 01 02 01 00 02 01 00 30 13 30 11 06 0d 2b 06 01 04 01 94 78 01 02 07 03 02 00 05 00");
	string tmp = pdu.generateResponsePacket(varBindList, requestID, errorStatus, errorIndex, "public");
	*///pdu.analyzePacket(tmp);
	//string response = pdu.packetHandler("30 26 02 01 00 04 06 70 75 62 6c 69 63 a0 19 02 02 04 d2 02 01 00 02 01 00 30 0d 30 0b 06 07 2b 06 01 02 01 01 01 05 00", OIDTree);
	//pdu.analyzePacket(response);
	string tmp = pdu.packetHandler("30 26 02 01 00 04 06 70 75 62 6c 69 63 a0 19 02 01 01 02 01 00 02 01 00 30 0e 30 0c 06 08 2b 06 01 02 01 01 01 00 05 00", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);


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