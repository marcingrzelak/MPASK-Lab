#pragma once

#pragma region errors
constexpr auto SOCKET_INIT_ERR = "Blad inicjalizacji socketa\n\n";
constexpr auto SOCKET_CREATE_ERR = "Blad podczas tworzenia socketa\n\n";
constexpr auto SOCKET_BIND_ERR = "Blad - socket bind\n\n";
constexpr auto SOCKET_LISTEN_ERR = "Blad - sluchanie na sockecie\n\n";
constexpr auto SOCKET_CONNECT_ERR = "Socket - blad polaczenia\n\n";
constexpr auto SERVER_TO_CLIENT_ERR = "Brak odpowiedzi z serwera\n\n";
#pragma endregion


#pragma region messages
constexpr auto CLIENT_PDU_ENTER = "Wpisz pakiet PDU\n";
#pragma endregion