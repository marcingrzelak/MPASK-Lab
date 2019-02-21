#include "pch.h"
#include "Network.h"
#include "Regex.h"
#include "Exceptions.h"
#include "Strings.h"
#include "PDUPackage.h"
#include "BERCoder.h"
#include "CheckValue.h"
#include "TreeStructure.h"

Network::Network()
{
}


Network::~Network()
{
}

void Network::socketInit()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		throw eSocketInitialization();
	}
	return;
}

void Network::socketCreate(SOCKET &pSocket)
{
	pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (pSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}
	return;
}

void Network::setSocketParam(const char* addr, u_short port, sockaddr_in &pSocketAddr)
{
	pSocketAddr.sin_family = AF_INET;
	pSocketAddr.sin_addr.s_addr = inet_addr(addr);
	pSocketAddr.sin_port = htons(port);
	return;
}

void Network::socketBind(SOCKET &pListenSocket, sockaddr_in &pSocketAddr)
{
	if (bind(pListenSocket, (SOCKADDR *)& pSocketAddr, sizeof(pSocketAddr)) == SOCKET_ERROR)
	{
		closesocket(pListenSocket);
		throw eSocketBind();
	}
	return;
}

void Network::socketListen(SOCKET &pListenSocket)
{
	if (listen(pListenSocket, 1) == SOCKET_ERROR)
	{
		throw eSocketListening();
	}
	return;
}

void Network::connectToServer(SOCKET &pSocket, sockaddr_in &pSocketAddr)
{
	if (connect(pSocket, (SOCKADDR *)& pSocketAddr, sizeof(pSocketAddr)) == SOCKET_ERROR)
	{
		WSACleanup();
		throw eSocketConnect();
	}
	return;
}

void Network::acceptConnection(SOCKET &pListenSocket, SOCKET &pServerSocket)
{
	while (pServerSocket == INVALID_SOCKET)
	{
		pServerSocket = accept(pListenSocket, NULL, NULL);
	}
	cout << SOCKET_CONNECTED;
	return;
}

void Network::clientSendPacket(SOCKET &pSocket, sockaddr_in &pSocketAddr)
{
	string commandString = "", OIDNumber, commandAsPDU;
	smatch result;
	regex rgx;
	PDUPackage pduPackage;
	map<string, string> varBindList;
	BERCoder encoder;
	CheckValue checkValue;
	int requestID;
	const char *sendBuffor = "";

	cout << CLIENT_COMMAND_ENTER;
	getline(cin, commandString);

	requestID = 1000 + (rand() % (14001)); //1000-15000

	//jaka komenda
	try
	{
		regex_search(commandString, result, Regex::snmpCommand());
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eNetwork();
	}

	if (result[1] == "snmpget" || result[1] == "snmpgetnext")
	{
		commandParsing(commandString, true);
		for (size_t i = 0; i < nodesOID.size(); i++)
		{
			try
			{
				OIDNumber = OIDtoNumber(nodesOID[i]);
			}
			catch (Exceptions &e)
			{
				e.message();
				throw eNetwork();
			}
			varBindList.insert(pair<string, string>(OIDNumber, encoder.nullEncode()));
		}
		if (result[1] == "snmpgetnext")
		{
			commandAsPDU = pduPackage.generatePacket(varBindList, GET_NEXT_REQUEST_TAG_NUMBER, requestID, 0, 0, community);
		}
		else
		{
			commandAsPDU = pduPackage.generatePacket(varBindList, GET_REQUEST_TAG_NUMBER, requestID, 0, 0, community);
		}
	}
	else if (result[1] == "snmpset")
	{
		commandParsing(commandString, false);

		for (size_t i = 0; i < nodesValue.size(); i++)
		{
			checkValue.setValueParameters(nodesValue[i]);
			if (checkValue.isValueNumber)
			{
				nodesValueEncoded.push_back(encoder.encode(nodesValue[i], INTEGER_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
			}
			else if (checkValue.isObjectIdentifier)
			{
				nodesValueEncoded.push_back(encoder.encode(nodesValue[i], OBJECT_IDENTIFIER_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
			}
			else
			{
				nodesValueEncoded.push_back(encoder.encode(nodesValue[i], OCTET_STRING_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
			}
		}

		for (size_t i = 0; i < nodesOID.size(); i++)
		{
			try
			{
				OIDNumber = OIDtoNumber(nodesOID[i]);
			}
			catch (Exceptions &e)
			{
				e.message();
				throw eNetwork();
			}
			varBindList.insert(pair<string, string>(OIDNumber, nodesValueEncoded[i]));
		}

		commandAsPDU = pduPackage.generatePacket(varBindList, SET_REQUEST_TAG_NUMBER, requestID, 0, 0, community);
	}
	else
	{
		throw eNetworkClientWrongCommand();
	}

	sendBuffor = commandAsPDU.c_str();

	try
	{
		connectToServer(pSocket, pSocketAddr);
	}
	catch (Exceptions &e)
	{
		throw eSocketConnect();
	}

	send(pSocket, sendBuffor, strlen(sendBuffor), 0);
	return;
}

string Network::clientReceivePacket(SOCKET &pSocket)
{
	int bytesRecv = 0;
	char recvBuffor[CLIENT_RECV_BUFFOR_SIZE] = "", secondBuffor[10] = "";
	clock_t begin = clock(), end;
	double elapsed_secs = 0;

	while (bytesRecv == 0)
	{
		bytesRecv = recv(pSocket, recvBuffor, CLIENT_RECV_BUFFOR_SIZE, 0);
		if (recv(pSocket, secondBuffor, 10, 0)) //przekroczony rozmiar bufora
		{
			throw eClientBufforOverflow();
		}

		if (bytesRecv < 0)
		{
			throw eServerToClient();
		}
		end = clock();
		elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		if (elapsed_secs > 5)
		{
			throw eServerNoResponse();
		}
	}
	closesocket(pSocket);
	return recvBuffor;
}

string Network::serverReceivePacket(SOCKET &pListenSocket, SOCKET &pServerSocket)
{
	char recvBuffor[SERVER_RECV_BUFFOR_SIZE] = "", secondBuffor[10] = "";

	acceptConnection(pListenSocket, pServerSocket);
	recv(pServerSocket, recvBuffor, SERVER_RECV_BUFFOR_SIZE, 0);
	//todo przekroczenie rozmiaru bufora

	cout << endl << "Klient -> Serwer: " << recvBuffor << endl;
	return recvBuffor;
}

void Network::serverSendPacket(SOCKET &serverSocket, string response)
{
	const char *sendBuffor = response.c_str();

	send(serverSocket, sendBuffor, strlen(sendBuffor), 0);
	cout << endl << "Serwer -> Klient: " << sendBuffor << endl;
	closesocket(serverSocket);
	PDUPackage pdu;
	pdu.analyzePacket(sendBuffor, true);
	cout << endl << endl;
	return;
}

void Network::commandParsing(string commandString, bool isSnmpGet)
{
	smatch result;
	regex rgx;
	try
	{
		regex_search(commandString, result, Regex::snmpGeneral());
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eNetwork();
	}

	if (result.size() != 5)
	{
		throw eNetworkClientWrongCommand();
	}

	command = result[iCommand];
	community = result[iCommunity];
	address = result[iAddress];
	nodesAll = result[iNodes];
	nodesAll.append(" ");

	if (isSnmpGet)
	{
		rgx = Regex::snmpgetOneElement();
	}
	else
	{
		rgx = Regex::snmpsetOneElement();
	}

	try
	{
		sregex_iterator nodesIterator(nodesAll.begin(), nodesAll.end(), rgx);
		sregex_iterator endIterator;

		if (nodesIterator == endIterator)
		{
			throw eNetworkClientWrongCommand();
		}

		while (nodesIterator != endIterator)
		{
			nodesOID.push_back((*nodesIterator)[1]);
			if (!isSnmpGet)
			{
				nodesValue.push_back((*nodesIterator)[2]);
			}
			++nodesIterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eNetwork();
	}
}

string Network::OIDtoNumber(string OID)
{
	CheckValue checkValue;
	string st;
	//sprawdzam czy oid jest liczba czy slowem
	checkValue.checkIsObjectIdentifier(OID);

	if (!checkValue.isObjectIdentifier)//nie jest liczba
	{
		//usuwanie ew 0 z konca oida
		if (OID.back() == '0')
		{
			st = OID.substr(0, OID.size() - 2);
		}
		else
		{
			st = OID;
		}

		//todo poki co brak mozliwosci odczytania numeru oid bo drzewo jest na serwerze
		//Tree OIDTree;
		//st = OIDTree.findNodeWord(st, OIDTree.root, "");	
		throw eOIDWord();
	}
	else
	{
		st = OID;
	}

	return st;
}
