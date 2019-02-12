#include "pch.h"
#include "Network.h"
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
}

void Network::socketCreate(SOCKET &pSocket)
{
	pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (pSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}
}

void Network::setSocketParam(const char* addr, u_short port, sockaddr_in &pSocketAddr)
{
	pSocketAddr.sin_family = AF_INET;
	pSocketAddr.sin_addr.s_addr = inet_addr(addr);
	pSocketAddr.sin_port = htons(port);
}

void Network::socketBind(SOCKET &pListenSocket, sockaddr_in &pSocketAddr)
{
	if (bind(pListenSocket, (SOCKADDR *)& pSocketAddr, sizeof(pSocketAddr)) == SOCKET_ERROR)
	{
		closesocket(pListenSocket);
		throw eSocketBind();
	}
}

void Network::socketListen(SOCKET &pListenSocket)
{
	if (listen(pListenSocket, 1) == SOCKET_ERROR)
	{
		throw eSocketListening();
	}
	cout << SOCKET_CONNECT_WAITING;
}

void Network::connectToServer(SOCKET &pSocket, sockaddr_in &pSocketAddr)
{
	if (connect(pSocket, (SOCKADDR *)& pSocketAddr, sizeof(pSocketAddr)) == SOCKET_ERROR)
	{
		WSACleanup();
		throw eSocketConnect();
	}
}

void Network::acceptConnection(SOCKET &pListenSocket, SOCKET &pServerSocket)
{
	while (pServerSocket == SOCKET_ERROR)
	{
		pServerSocket = accept(pListenSocket, NULL, NULL);
	}
	cout << SOCKET_CONNECTED;
}

void Network::clientSendPacket(SOCKET &pSocket, sockaddr_in &pSocketAddr)
{
	string pduString = "";
	const char *sendBuffor = "";

	cout << CLIENT_PDU_ENTER << endl;
	cin >> pduString;
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

	cout << "Odpowiedz serwera: " << recvBuffor << endl;
	closesocket(pSocket);
}

void Network::serverReceivePacket(SOCKET &pListenSocket, SOCKET &pServerSocket)
{
	char recvBuffor[SERVER_RECV_BUFFOR_SIZE] = "";

	acceptConnection(pListenSocket, pServerSocket);

	recv(pServerSocket, recvBuffor, SERVER_RECV_BUFFOR_SIZE, 0);
	cout << "Client -> Serwer: " << recvBuffor << endl;
}

void Network::serverSendPacket(SOCKET &serverSocket)
{
	char sendBuffor[SERVER_SEND_BUFFOR_SIZE] = "odp serwera";

	send(serverSocket, sendBuffor, strlen(sendBuffor), 0);
	cout << "Serwer -> Client: " << sendBuffor << endl;
}