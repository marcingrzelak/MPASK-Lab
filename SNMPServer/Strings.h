#pragma once

#pragma region errors
#define F_OPEN_ERR_MSG "Blad otwarcia pliku."<<endl<<endl

#pragma endregion

#pragma region messages

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


#pragma endregion

