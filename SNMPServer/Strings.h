#pragma once

#pragma region errors
constexpr auto FILE_OPEN_ERR_MSG = "Blad otwarcia pliku\n\n";
constexpr auto PARSER_ERR_MSG = "Blad parsera\n\n";
constexpr auto PARSER_STOI_ERR_MSG = "Blad parsera - konwersja stringa na liczbe\n\n";
constexpr auto ADD_NODE_PARENT_NULL_ERR = "Probowano dodac wezel do niezainicjalizowanego rodzica\n\n";
constexpr auto FIND_OID_PARENT_NULL_ERR= "Probowano wyszukac element w pustym drzewie\n\n";
constexpr auto FIND_NODE_WORD_PARENT_NULL_ERR = "Probowano wyszukac element w pustym drzewie\n\n";
//constexpr auto = "\n\n";
//constexpr auto = "\n\n";
//constexpr auto = "\n\n";
//constexpr auto = "\n\n";

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

constexpr int PDU_ERR_NO_ERROR = 0;
constexpr int PDU_ERR_TOO_BIG = 1;
constexpr int PDU_ERR_NO_SUCH_NAME = 2;
constexpr int PDU_ERR_BAD_VALUE = 3;
constexpr int PDU_ERR_READ_ONLY = 4;
constexpr int PDU_ERR_GEN_ERR = 5;

constexpr auto READ_ONLY = "read-only";

//constexpr auto comments = R"(-{2}.*)";
//constexpr auto whitespaces = R"(\s{2,})";
//constexpr auto importsGeneral = R"(IMPORTS\s(.*?);)";
//constexpr auto imports = R"((.*?)\sFROM\s(\S*))";
//constexpr auto importsOneElement = R"(\s?(.*?),)";
//constexpr auto objectIdentifierGeneral = R"((\S*)\sOBJECT\sIDENTIFIER\s::=\s\{\s(.*?)\s(\d+)\s\})";
//constexpr auto objectIdentifierLongParent = R"((\S*?)(\((\d+)\))?\s)";
//constexpr auto objectType = R"((\w*)\sOBJECT-TYPE\sSYNTAX\s(.*?)\sACCESS\s(.*?)\sSTATUS\s(.*?)\sDESCRIPTION\s(.*?)\s(INDEX\s\{.*?\}\s)?::=\s\{\s(\S*)\s(\d+)\s\})";
//constexpr auto sizeGeneral = R"(\(.*?\)+)";
//constexpr auto sizeR = R"(\(SIZE\((\d+)\)\)|\((\d+)\.\.(\d+)\))";
//constexpr auto index = R"(\{\s(.*?)\s\})";
//constexpr auto indexOneElement = R"(\s?(.*?),)";
//constexpr auto dataType = R"((\w*?)\s::=\s?\[?(\S*?)?\s?(\d+)?\]?\s?(\S*?)?\s(INTEGER|OCTET STRING|OBJECT IDENTIFIER|NULL)\s\(?(((SIZE)?\s\((\d+)\))|((\d+)\.{2}(\d+)))?\)?)";
//constexpr auto dataTypeEncoder = R"((INTEGER)|(OCTET STRING)|(OBJECT IDENTIFIER)|(NULL))";
//constexpr auto choiceGeneral = R"((\w*)\s::=\s?CHOICE\s\{\s(.*?)\s\})";
//constexpr auto choiceOneElement = R"(\s?(\S*?)\s(.*?),)";
//constexpr auto sequenceGeneral = R"((\w*)\s::=\sSEQUENCE\s\{(.*?)\s\})";
//constexpr auto sequenceOneElement = R"(\s?(\S*?)\s(.*?),)";
//constexpr auto objectIdentifierCheckType = R"(\d+\.{1})";


#pragma endregion

