#include "pch.h"
#include <iostream>
#include "../SNMPServer/Strings.h"
#include "../SNMPServer/Exceptions.h"
#include "../SNMPServer/Network.h"

int main()
{
	//Network net;

	//net.setClient("127.0.0.1", 161);


	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		//throw eSocketInitialization();
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		WSACleanup();
		//throw eSocketCreate();
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(161);

	string pduString = "";
	int bytesSent, bytesRecv;
	char *recvBuffor;
	const char *sendBuffor;

	while (true)
	{
		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;
		cout << CLIENT_PDU_ENTER << endl;
		cin >> pduString;

		sendBuffor = pduString.c_str();

		if (connect(serverSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
		{
			WSACleanup();
			//throw eSocketConnect();
		}

		bytesSent = send(serverSocket, sendBuffor, strlen(sendBuffor), 0);
		cout << "Bytes sent: " << bytesSent << endl;

		closesocket(serverSocket);

		serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (serverSocket == INVALID_SOCKET)
		{
			WSACleanup();
			//throw eSocketCreate();
		}
	}


	//while (bytesRecv == SOCKET_ERROR)
	//{
	//	bytesRecv = recv(serverSocket, recvBuffor, 32, 0);

	//	if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
	//	{
	//		cout << "Connection closed." << endl;
	//		break;
	//	}

	//	if (bytesRecv < 0)
	//		return 1;

	//	cout << "Bytes received: " << bytesRecv << endl;
	//	cout << "Received text: " << recvBuffor << endl;
	//}

	system("pause");
	return (0);
}