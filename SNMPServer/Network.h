#pragma once
class Network
{
public:
	Network();
	~Network();
	void setServer(const char* addr, u_short port);
	void setClient(const char* addr, u_short port);
};

