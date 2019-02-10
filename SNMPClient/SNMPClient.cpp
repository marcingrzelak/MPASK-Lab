#include "pch.h"
#include "Strings.h"
#include "Exceptions.h"

#define SEND_BUFFOR_SIZE 256
#define RECV_BUFFOR_SIZE 256

SOCKET clientSocket;

void socketInit()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		throw eSocketInitialization();
	}
}

void socketCreate()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw eSocketCreate();
	}
}

void sendPacket(const char* addr, u_short port)
{
	sockaddr_in service;
	string pduString = "";
	int bytesSent = 0;
	const char *sendBuffor = "";

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(addr);
	service.sin_port = htons(port);

	cout << CLIENT_PDU_ENTER << endl;
	cin >> pduString;

	sendBuffor = pduString.c_str();

	if (connect(clientSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		WSACleanup();
		throw eSocketConnect();
	}

	bytesSent = send(clientSocket, sendBuffor, strlen(sendBuffor), 0);
}

void receivePacket()
{
	int bytesRecv = 0;
	char recvBuffor[RECV_BUFFOR_SIZE] = "";

	while (bytesRecv == 0)
	{
		bytesRecv = recv(clientSocket, recvBuffor, RECV_BUFFOR_SIZE, 0);

		if (bytesRecv < 0)
		{
			throw eServerToClient();
		}
	}

	cout << "Odpowiedz serwera: " << recvBuffor << endl;
	closesocket(clientSocket);
}

int main()
{
	try
	{
		socketInit();
		socketCreate();
		sendPacket("127.0.0.1", 161);
		receivePacket();
	}
	catch (Exceptions &e)
	{
		e.message();
		return 1;
	}

	system("pause");
	return (0);
}