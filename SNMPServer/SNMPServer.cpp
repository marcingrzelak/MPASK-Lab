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
#include "Network.h"


void showImportedObjects(vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence)
{
	DataType::showDataTypes(vDataType);
	Index::showIndexes(vIndex);
	Choice::showChoices(vChoice);
	Sequence::showSequences(vSequence);
	return;
}

void printOIDTree(Tree OIDTree)
{
	cout << "Drzewo OID" << endl;
	OIDTree.root->printTree("", true);
	return;
}

void findNodeByOID(string oid, Tree OIDTree)
{
	TreeNode * node;
	try
	{
		node = OIDTree.findOID(oid, OIDTree.root);
	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}
	cout << node->name << endl;
	return;
}

void findNodeByName(string name, Tree OIDTree)
{
	string oid;

	try
	{
		oid = OIDTree.findNodeWord(name, OIDTree.root, "");
	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}

	if (oid.size() == 0)
	{
		cout << "Brak wezla o podanej nazwie" << endl;
	}
	else
	{
		cout << "OID wezla o podanej nazwie: " << oid << endl;
	}
	return;
}

string encode(BERCoder coder, Tree OIDTree, vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence, vector < ObjectTypeSize> vObjectTypeSize)
{
	int encodingType = 0;
	string encodedValue, encodedValue1;

	cout << "Wybierz typ kodowania:" << endl << "1 - kodowanie liscia drzewa" << endl << "2 - kodowanie dowolnej wartosci" << endl;
	cin >> encodingType;

	if (encodingType == 1)
	{
		encodedValue = coder.treeNodeEncoding("", "", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		cout << endl << "Zakodowana wartosc:" << endl;
		cout << encodedValue << endl;
	}
	else if (encodingType == 2)
	{
		encodedValue1 = "", encodedValue = coder.anyValueEncoding(encodedValue1, false);
		cout << endl << "Zakodowana wartosc:" << endl;
		cout << encodedValue << endl;
	}
	else
	{
		cout << "Podano zly typ" << endl;
	}
	return encodedValue;
}

void decode(string encodedValue, BERDecoder decoder, TreeBER BERTree)
{
	decoder.decode(encodedValue, 0, BERTree, nullptr);
	cout << endl << "Odkodowana wartosc:" << endl;
	BERTree.root->printTree("", true);
	cout << endl;
}

void pduAnalyzePacket(PDUPackage pdu, string packet, TreeBER BERTree)
{
	//"30 2c 02 01 00 04 07 70 72 69 76 61 74 65 a0 1e 02 01 01 02 01 00 02 01 00 30 13 30 11 06 0d 2b 06 01 04 01 94 78 01 02 07 03 02 00 05 00"
	pdu.analyzePacket(packet, true);
	BERTree.root->printTree("", true);
	return;
}

void pduGenerateResponse(string oid, string value, int requestID, int errorStatus, int errorIndex, string community, PDUPackage pdu)
{
	map<string, string> varBindList;
	//oid "1.3.6.1.2.1.1.1.0"
	//value "05 00"
	varBindList.insert(pair<string, string>(oid, value));
	string responsePacket = pdu.generatePacket(varBindList, GET_RESPONSE_MY_TAG, requestID, errorStatus, errorIndex, community);
	cout << "Odpowiedz: " << responsePacket << endl;
	return;
}

void pduGenerateResponseFromPacket(PDUPackage pdu, string packet, Tree OIDTree, vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence, vector < ObjectTypeSize> vObjectTypeSize)
{
	//packet "30 26 02 01 00 04 06 70 75 62 6c 69 63 a0 19 02 01 01 02 01 00 02 01 00 30 0e 30 0c 06 08 2b 06 01 02 01 01 01 00 05 00"
	string responsePacket = pdu.packetHandler(packet, OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
	cout << "Odpowiedz: " << responsePacket << endl;
	return;
}

void runServer(Network server, const char* addr, u_short port, SOCKET &listenSocket, sockaddr_in socketAddr)
{
	try
	{
		server.socketInit();
		server.socketCreate(listenSocket);
		server.setSocketParam(addr, port, socketAddr);
		server.socketBind(listenSocket, socketAddr);
		server.socketListen(listenSocket);

	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}
}

void dataTransmissionServer(Network server, SOCKET &listenSocket, SOCKET serverSocket, Tree &OIDTree, vector<DataType>& pVDataType, vector<Index>& pVIndex, vector<Choice>& pVChoice, vector<Sequence>& pVSequence, vector<ObjectTypeSize>& pVObjectTypeSize)
{
	PDUPackage pdu;
	try
	{
		cout << SOCKET_CONNECT_WAITING;
		string packet = server.serverReceivePacket(listenSocket, serverSocket);
		string response = pdu.packetHandler(packet, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
		server.serverSendPacket(serverSocket, response);

	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}
}

int main()
{
	Tree OIDTree;
	TreeBER BERTree;
	Parser parser;
	BERCoder coder;
	BERDecoder decoder;
	PDUPackage pduPackage;
	Network server;
	SOCKET listenSocket = SOCKET_ERROR, serverSocket = SOCKET_ERROR;
	sockaddr_in socketAddr;

	vector <DataType> vDataType;
	vector <Index> vIndex;
	vector <Choice> vChoice;
	vector <Sequence> vSequence;
	vector <ObjectTypeSize> vObjectTypeSize;

	SetConsoleTitle(TEXT("SNMP Server"));

	memset(&socketAddr, 0, sizeof(socketAddr));

	try
	{
		parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		//string e = encode(coder, OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		//decode(e, decoder, BERTree);
		//pduGenerateResponse("1.3.6.1.2.1.1.1.0", "05 00", 1234, 0, 0, "public", pduPackage);
		runServer(server, "127.0.0.1", 161, listenSocket, socketAddr);

		while (true)
		{
			serverSocket = SOCKET_ERROR;
			dataTransmissionServer(server, listenSocket, serverSocket, OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		}
	}
	catch (Exceptions &e)
	{
		e.message();
		return -1;
	}

	//podawanie wartosci sekwencji w formacie {w1,w2,w3,...,wn}
	//podawanie wartosci object identifier w formacie {1.2.3.4.5}

	return(0);
}