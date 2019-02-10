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

void Network::socketCreate()
{
	mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}
}

void Network::socketBind(const char* addr, u_short port)
{
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(addr);
	service.sin_port = htons(port);

	if (bind(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(mainSocket);
		throw eSocketBind();
	}
}

void Network::socketListen()
{
	if (listen(mainSocket, 1) == SOCKET_ERROR)
	{
		throw eSocketListening();
	}
}

void Network::receivePacket()
{
	clientSocket = SOCKET_ERROR;
	int bytesRecv = SOCKET_ERROR;
	char recvBuffor[RECV_BUFFOR_SIZE] = "";

	cout << SOCKET_CONNECT_WAITING;

	while (clientSocket == SOCKET_ERROR)
	{
		clientSocket = accept(mainSocket, NULL, NULL);
	}

	cout << SOCKET_CONNECTED;

	bytesRecv = recv(clientSocket, recvBuffor, RECV_BUFFOR_SIZE, 0);
	cout << "Client -> Serwer: " << recvBuffor << endl;

}

void Network::sendPacket()
{
	int bytesSent;
	char sendBuffor[SEND_BUFFOR_SIZE] = "odp serwera";

	bytesSent = send(clientSocket, sendBuffor, strlen(sendBuffor), 0);
	cout << "Serwer -> Client: " << sendBuffor << endl;
}