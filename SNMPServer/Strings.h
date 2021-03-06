#pragma once

#pragma region errors
constexpr auto FILE_OPEN_ERR_MSG = "Blad otwarcia pliku\n\n";
constexpr auto PARSER_ERR_MSG = "Blad parsera\n\n";
constexpr auto PARSER_STOI_ERR_MSG = "Blad parsera - konwersja stringa na liczbe\n\n";
constexpr auto ADD_NODE_PARENT_NULL_ERR = "Probowano dodac wezel do niezainicjalizowanego rodzica\n\n";
constexpr auto FIND_OID_PARENT_NULL_ERR= "Probowano wyszukac element w pustym drzewie\n\n";
constexpr auto FIND_NODE_WORD_PARENT_NULL_ERR = "Probowano wyszukac element w pustym drzewie\n\n";
constexpr auto ENCODING_INPUT_TYPE_ERR = "Podano zly numer typu danych\n\n";
constexpr auto ENCODING_TYPE_ERR = "Blad typu danych\n\n";
constexpr auto ENCODING_SIZE_ERR = "Blad rozmiaru danych\n\n";
constexpr auto ENCODER_ERR_MSG = "Blad kodera\n\n";
constexpr auto NODE_NOT_FOUND = "Nie znaleziono liscia\n\n";
constexpr auto UNKNOWN_CLASS_ERR = "Niepoprawny typ klasy w bajcie identyfikatora\n\n";
constexpr auto UNKNOWN_COMPLEXITY_ERR = "Typ nie jest ani prosty, ani zlozony\n\n";
constexpr auto WRONG_TAG_ERR = "Tag nie moze miec wartosci 31\n\n";
constexpr auto UNKNOWN_TAG_TYPE_ERR = "Nie rozpoznano tagu\n\n";
constexpr auto LENGTH_K_ERR = "Niepoprawna ilosc oktetow (K) pola dlugosc\n\n";
constexpr auto LENGTH_ERR = "Pole dlugosci nie moze miec 128 bajtow\n\n";
constexpr auto VALUE_OBJECT_IDENTIFIER_ERR = "Niepoprawne dane dla typu object identifier\n\n";
constexpr auto DECODER_ERR_MSG = "Blad dekodera\n\n";
constexpr auto SOCKET_INIT_ERR = "Blad inicjalizacji socketa\n\n";
constexpr auto SOCKET_CREATE_ERR = "Blad podczas tworzenia socketa\n\n";
constexpr auto SOCKET_BIND_ERR = "Blad - socket bind\n\n";
constexpr auto SOCKET_LISTEN_ERR = "Blad - sluchanie na sockecie\n\n";
constexpr auto SOCKET_CONNECT_ERR = "Socket - blad polaczenia \n\n";
constexpr auto SERVER_TO_CLIENT_ERR = "Brak odpowiedzi z serwera\n\n";
constexpr auto NETWORK_ERR_MSG = "Blad sieci\n\n";
constexpr auto PDU_ERR_MSG = "Blad w obsludze PDU\n\n";
constexpr auto NETWORK_CLIENT_WRONG_COMMAND = "Bledna skladnia polecenia\n\n";
constexpr auto WRONG_TAG_GENERATE_PDU = "Niepoprawna komenda snmp\n\n";
constexpr auto OID_WORD_ERROR= "Nie obsluguje OIDow jako wyrazy. Wprowadz liczbowy OID\n\n";
constexpr auto OID_WRONG_SYNTAX_ERROR = "Niepoprawna skladnia OID\n\n";
constexpr auto GET_NEXT_END_ERROR = "Osiagnieto koniec drzewa\n\n";
constexpr auto CLIENT_BUFFOR_OVERFLOW_ERROR = "Przekroczono rozmiar bufora klienta\n\n";
constexpr auto SERVER_BUFFOR_OVERFLOW_ERROR = "Przekroczono rozmiar bufora serwera\n\n";
constexpr auto SET_VALUE_NOT_CHANGED_ERROR = "Nie udalo sie ustawic nowej wartosci\n\n";
constexpr auto SET_VALUE_NOT_IMPLEMENTED_ERROR = "Brak implementacji ustawiania wartosci tego wezla\n\n";
constexpr auto SERVER_NO_RESPONSE_ERROR = "Brak odpowiedzi z serwera\n\n";
#pragma endregion

#pragma region messages
constexpr auto SOCKET_CONNECT_WAITING = "Oczekuje na polaczenie od klienta...\n";
constexpr auto SOCKET_CONNECTED = "Klient podlaczony...\n";
constexpr auto CLIENT_COMMAND_ENTER = "Wpisz polecenie snmp\n";
#pragma endregion

#pragma region oids
constexpr auto SYS_NAME_OID = "1.3.6.1.2.1.1.5";
constexpr auto SYS_UP_TIME_OID = "1.3.6.1.2.1.1.3";
#pragma endregion


#pragma region constants
constexpr auto IDENTIFIER_CLASS_UNIVERSAL = "UNIVERSAL";
constexpr auto IDENTIFIER_CLASS_APPLICATION = "APPLICATION";
constexpr auto IDENTIFIER_CLASS_CONTEXT_SPECIFIC = "CONTEXT-SPECIFIC";
constexpr auto IDENTIFIER_CLASS_PRIVATE = "PRIVATE";
constexpr auto IDENTIFIER_COMPLEXITY_PRIMITIVE = "PRIMITIVE";
constexpr auto IDENTIFIER_COMPLEXITY_CONSTRUCTED = "CONSTRUCTED";
constexpr auto IDENTIFIER_TYPE_INTEGER = "INTEGER";
constexpr auto IDENTIFIER_TYPE_OCTET_STRING = "OCTET STRING";
constexpr auto IDENTIFIER_TYPE_NULL = "NULL";
constexpr auto IDENTIFIER_TYPE_OBJECT_IDENTIFIER = "OBJECT IDENTIFIER";
constexpr auto IDENTIFIER_TYPE_SEQUENCE = "SEQUENCE";
constexpr auto IDENTIFIER_TYPE_SEQUENCE_OF = "SEQUENCE OF";
constexpr unsigned int IDENTIFIER_TAG_LONG = 0x1F;
constexpr auto DATATYPE_KEYWORD_IMPLICIT = "IMPLICIT";
constexpr auto DATATYPE_KEYWORD_EXPLICIT = "EXPLICIT";

constexpr int INTEGER_TAG_NUMBER = 2;
constexpr int OCTET_STRING_TAG_NUMBER = 4;
constexpr int NULL_TAG_NUMBER = 5;
constexpr int OBJECT_IDENTIFIER_TAG_NUMBER = 6;
constexpr int SEQUENCE_TAG_NUMBER = 16;

constexpr auto PACKET_TYPE_GET_REQUEST = "GetRequest";
constexpr auto PACKET_TYPE_GET_RESPONSE = "GetResponse";
constexpr auto PACKET_TYPE_SET_REQUEST = "SetRequest";
constexpr auto PACKET_TYPE_GET_NEXT_REQUEST = "GetNextRequest";

constexpr int GET_REQUEST_TAG_NUMBER = 0;
constexpr int GET_RESPONSE_TAG_NUMBER = 2;
constexpr int SET_REQUEST_TAG_NUMBER = 3;
constexpr int GET_NEXT_REQUEST_TAG_NUMBER = 1;

constexpr unsigned short GET_REQUEST_BYTE = 0xa0;
constexpr unsigned short GET_RESPONSE_BYTE = 0xa2;
constexpr unsigned short SET_REQUEST_BYTE = 0xa3;
constexpr unsigned short GET_NEXT_REQUEST_BYTE = 0xa1;

constexpr int GET_REQUEST_MY_TAG = 0x80;
constexpr int GET_RESPONSE_MY_TAG = 0x82;
constexpr int SET_REQUEST_MY_TAG = 0x83;
constexpr int GET_NEXT_REQUEST_MY_TAG = 0x81;

constexpr int PDU_ERR_NO_ERROR_CODE = 0;
constexpr int PDU_ERR_TOO_BIG_CODE = 1;
constexpr int PDU_ERR_NO_SUCH_NAME_CODE = 2;
constexpr int PDU_ERR_BAD_VALUE_CODE = 3;
constexpr int PDU_ERR_READ_ONLY_CODE = 4;
constexpr int PDU_ERR_GEN_ERR_CODE = 5;

constexpr auto PDU_ERR_TOO_BIG = "tooBig";
constexpr auto PDU_ERR_NO_SUCH_NAME = "noSuchName";
constexpr auto PDU_ERR_BAD_VALUE = "badValue";
constexpr auto PDU_ERR_READ_ONLY = "readOnly";
constexpr auto PDU_ERR_GEN_ERR = "genErr";

constexpr auto PDU_ERROR_IN_PACKET = "Blad w pakiecie";
constexpr auto ERR_REASON = "Powod: ";

constexpr auto READ_ONLY = "read-only";
#pragma endregion