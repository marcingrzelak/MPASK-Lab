#include "pch.h"
#include "Network.h"
#include "Regex.h"
#include "Exceptions.h"
#include "Strings.h"

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
	string pduString = "";
	smatch result;
	regex rgx;
	const char *sendBuffor = "";

	cout << CLIENT_PDU_ENTER << endl;
	getline(cin, pduString);

	try
	{
		regex_search(pduString, result, Regex::snmpGeneral());
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eNetwork();
	}

	if (result.size() != 4)
	{
		throw eNetworkClientWrongCommand();
	}
	command = result[iCommand];
	community = result[iCommunity];
	nodesAll = result[iNodes];
	nodesAll.append(" ");

	try
	{
		rgx = Regex::snmpOneElement();
		sregex_iterator nodesIterator(nodesAll.begin(), nodesAll.end(), rgx);
		sregex_iterator endIterator;

		while (nodesIterator != endIterator)
		{
			nodes.push_back((*nodesIterator)[1]);
			++nodesIterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eNetwork();
	}

	//todo zbudowac pakiet pdu dla podanych danych
	//todo wyslanie pakietu pdu


	sendBuffor = pduString.c_str();


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

void Network::clientReceivePacket(SOCKET &pSocket)
{
	int bytesRecv = 0;
	char recvBuffor[CLIENT_RECV_BUFFOR_SIZE] = "";

	while (bytesRecv == 0)
	{
		bytesRecv = recv(pSocket, recvBuffor, CLIENT_RECV_BUFFOR_SIZE, 0);

		if (bytesRecv < 0)
		{
			throw eServerToClient();
		}
	}
	//todo analiza odebranego pakietu
	cout << "Odpowiedz serwera: " << recvBuffor << endl;
	closesocket(pSocket);
	return;
}

void Network::serverReceivePacket(SOCKET &pListenSocket, SOCKET &pServerSocket)
{
	char recvBuffor[SERVER_RECV_BUFFOR_SIZE] = "";

	acceptConnection(pListenSocket, pServerSocket);

	recv(pServerSocket, recvBuffor, SERVER_RECV_BUFFOR_SIZE, 0);
	cout << "Client -> Serwer: " << recvBuffor << endl;
	//todo analiza odebranego pakietu
	//todo odpalenie packet handler z PDUPackage.cpp
	return;
}

void Network::serverSendPacket(SOCKET &serverSocket)
{
	//todo wyslanie odpowiedzi (pakietu pdu) z packethandlera
	char sendBuffor[SERVER_SEND_BUFFOR_SIZE] = "odp serwera";

	send(serverSocket, sendBuffor, strlen(sendBuffor), 0);
	cout << "Serwer -> Client: " << sendBuffor << endl;
	return;
}