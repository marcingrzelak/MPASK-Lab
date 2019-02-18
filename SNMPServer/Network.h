#pragma once

#define SERVER_SEND_BUFFOR_SIZE 256
#define SERVER_RECV_BUFFOR_SIZE 256
#define CLIENT_SEND_BUFFOR_SIZE 256
#define CLIENT_RECV_BUFFOR_SIZE 256

class Network
{
public:
	Network();
	~Network();
	void socketInit();
	void socketCreate(SOCKET &pSocket);
	void setSocketParam(const char* addr, u_short port, sockaddr_in &pSocketAddr);
	void socketBind(SOCKET &pServerSocket, sockaddr_in &pSocketAddr);
	void socketListen(SOCKET &pServerSocket);
	void connectToServer(SOCKET &pSocket, sockaddr_in &pSocketAddr);
	void acceptConnection(SOCKET &pListenSocket, SOCKET &pServerSocket);
	void clientSendPacket(SOCKET &pSocket, sockaddr_in &pSocketAddr);
	string clientReceivePacket(SOCKET &pSocket);
	string serverReceivePacket(SOCKET &pListenSocket, SOCKET &pServerSocket);
	void serverSendPacket(SOCKET &pServerSocket, string response);

	void commandParsing(string commandString, bool isSnmpGet);
	string OIDtoNumber(string OID);

	short iCommand = 1, iCommunity = 2, iAddress = 3, iNodes = 4;
	string command, community, address, nodesAll;
	vector<string> nodesOID, nodesValue, nodesValueEncoded;
};