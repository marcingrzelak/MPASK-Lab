#include "pch.h"
#include <iostream>

int main()
{
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Client says hello!";
	char recvbuf[32] = "";

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
		return 1;
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(161);

	if (connect(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "Failed to connect." << endl;
		WSACleanup();
		return 1;
	}

	bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
	cout << "Bytes sent: " << bytesSent << endl;

	while (bytesRecv == SOCKET_ERROR)
	{
		bytesRecv = recv(mainSocket, recvbuf, 32, 0);

		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			cout << "Connection closed." << endl;
			break;
		}

		if (bytesRecv < 0)
			return 1;

		cout << "Bytes received: " << bytesRecv << endl;
		cout << "Received text: " << recvbuf << endl;
	}

	system("pause");
	return (0);
}