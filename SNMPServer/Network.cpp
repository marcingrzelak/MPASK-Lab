#include "pch.h"
#include "Network.h"


Network::Network()
{
}


Network::~Network()
{
}

void Network::setServer()
{
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		cout << "Initialization error." << endl;
	}

	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		cout << "Error creating socket: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(161);

	if (bind(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "bind() failed." << endl;
		closesocket(mainSocket);
		return;
	}

	if (listen(mainSocket, 1) == SOCKET_ERROR)
	{
		cout << "Error listening on socket." << endl;
	}

	SOCKET clientSocket;
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[256] = "";
	char recvbuf[128] = "";

	bool exit = false;

	while (!exit)
	{
		clientSocket = SOCKET_ERROR;

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;
		memset(sendbuf, 0, sizeof sendbuf);
		memset(recvbuf, 0, sizeof recvbuf);

		cout << "Waiting for a client to connect..." << endl;

		while (clientSocket == SOCKET_ERROR)
		{
			if (GetAsyncKeyState(VK_ESCAPE)!=0)
			{
				exit = true;
				break;
			}

			clientSocket = accept(mainSocket, NULL, NULL);
		}

		cout << "Client connected." << endl;

		int bytesSent;
		int bytesRecv = SOCKET_ERROR;
		char sendbuf[256] = "";
		char recvbuf[128] = "";

		bytesRecv = recv(clientSocket, recvbuf, 128, 0);
		printf("Bytes received: %ld\n", bytesRecv);
		printf("Received text: %s\n", recvbuf);

		//bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
		//printf("Bytes sent: %ld\n", bytesSent);
		//printf("Bytes sent: %ld\n", bytesSent);
	}
	system("pause");
}
