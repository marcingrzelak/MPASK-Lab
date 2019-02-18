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
	PDUPackage pdu;
	string response;
	try
	{
		client.socketInit();
		client.socketCreate(clientSocket);
		client.setSocketParam(addr, port, socketAddr);
		client.clientSendPacket(clientSocket, socketAddr);
		response = client.clientReceivePacket(clientSocket);
		pdu.analyzePacket(response, false);

		if (pdu.errorIndex == 0 && pdu.errorStatus == PDU_ERR_NO_ERROR_CODE)
		{
			pdu.printResponse();
		}
		else if (pdu.errorStatus != PDU_ERR_NO_ERROR_CODE)
		{
			cout << PDU_ERROR_IN_PACKET << endl << ERR_REASON;
			
			if (pdu.errorStatus == PDU_ERR_TOO_BIG_CODE)
			{
				cout << PDU_ERR_TOO_BIG << endl;
			}
			else if (pdu.errorStatus == PDU_ERR_NO_SUCH_NAME_CODE)
			{
				cout << PDU_ERR_NO_SUCH_NAME << endl;
			}
			else if (pdu.errorStatus == PDU_ERR_BAD_VALUE_CODE)
			{
				cout << PDU_ERR_BAD_VALUE << endl;
			}
			else if (pdu.errorStatus == PDU_ERR_READ_ONLY_CODE)
			{
				cout << PDU_ERR_READ_ONLY << endl;
			}
			else if (pdu.errorStatus == PDU_ERR_GEN_ERR_CODE)
			{
				cout << PDU_ERR_GEN_ERR << endl;
			}
			cout << endl;
		}
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