#include "pch.h"
#include "../SNMPServer/Network.h"
#include "../SNMPServer/Exceptions.h"
#include "../SNMPServer/Regex.h"
#include "../SNMPServer/PDUPackage.h"
#include "../SNMPServer/BERCoder.h"
#include "../SNMPServer/BERDecoder.h"
#include "../SNMPServer/CheckValue.h"
#include "../SNMPServer/DataStructures.h"
#include "../SNMPServer/Identifier.h"
#include "../SNMPServer/Length.h"
#include "../SNMPServer/TreeStructure.h"
#include "../SNMPServer/Value.h"


void runClient(Network client, const char* addr, u_short port, SOCKET clientSocket, sockaddr_in socketAddr)
{
	try
	{
		client.socketInit();
		client.socketCreate(clientSocket);
		client.setSocketParam(addr, port, socketAddr);
		client.clientSendPacket(clientSocket, socketAddr);
		client.clientReceivePacket(clientSocket);
	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}
}

int main()
{
	SetConsoleTitle(TEXT("SNMP Client"));

	Network client;
	srand(time(NULL));

	while (true)
	{
		SOCKET clientSocket = SOCKET_ERROR;
		sockaddr_in socketAddr;
		memset(&socketAddr, 0, sizeof(socketAddr));
		runClient(client, "127.0.0.1", 161, clientSocket, socketAddr);
	}

	system("pause");
	return (0);
}