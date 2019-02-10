#pragma once

#define SEND_BUFFOR_SIZE 256
#define RECV_BUFFOR_SIZE 256

class Network
{
public:
	Network();
	~Network();
	void socketInit();
	void socketCreate();
	void socketBind(const char* addr, u_short port);
	void socketListen();
	void receivePacket();
	void sendPacket();
	SOCKET mainSocket, clientSocket;
};