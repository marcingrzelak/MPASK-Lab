#include "pch.h"
#include "Exceptions.h"
#include "Strings.h"

void eSocketInitialization::message()
{
	cout << SOCKET_INIT_ERR;
}

void eSocketCreate::message()
{
	cout << SOCKET_CREATE_ERR;
}

void eSocketBind::message()
{
	cout << SOCKET_BIND_ERR;
}

void eSocketListening::message()
{
	cout << SOCKET_LISTEN_ERR;
}

void eSocketConnect::message()
{
	cout << SOCKET_CONNECT_ERR;
}

void eServerToClient::message()
{
	cout << SERVER_TO_CLIENT_ERR;
}
