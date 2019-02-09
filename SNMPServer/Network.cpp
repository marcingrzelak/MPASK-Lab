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

void Network::setServer(const char* addr, u_short port)
{
	#define SEND_BUFFOR_SIZE 256
	#define RECV_BUFFOR_SIZE 128

	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		throw eSocketInitialization();
	}

	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}

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

	if (listen(mainSocket, 1) == SOCKET_ERROR)
	{
		throw eSocketListening();
	}

	SOCKET clientSocket;
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendBuffor[SEND_BUFFOR_SIZE] = "";
	char recvBuffor[RECV_BUFFOR_SIZE] = "";

	while (true)
	{
		clientSocket = SOCKET_ERROR;

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;
		memset(sendBuffor, 0, sizeof sendBuffor);
		memset(recvBuffor, 0, sizeof recvBuffor);

		cout << SOCKET_CONNECT_WAITING;

		while (clientSocket == SOCKET_ERROR)
		{
			clientSocket = accept(mainSocket, NULL, NULL);
		}

		cout << SOCKET_CONNECTED;

		bytesRecv = recv(clientSocket, recvBuffor, RECV_BUFFOR_SIZE, 0);
		cout << "Bytes received: " << bytesRecv << endl;
		cout << "Received text: " << recvBuffor << endl;

		//bytesSent = send(mainSocket, sendBuffor, strlen(sendBuffor), 0);
		//printf("Bytes sent: %ld\n", bytesSent);
		//printf("Bytes sent: %ld\n", bytesSent);
	}
	system("pause");
}

void Network::setClient(const char * addr, u_short port)
{
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		throw eSocketInitialization();
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(addr);
	service.sin_port = htons(port);

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
			throw eSocketConnect();
		}

		bytesSent = send(serverSocket, sendBuffor, strlen(sendBuffor), 0);
		cout << "Bytes sent: " << bytesSent << endl;
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
}
