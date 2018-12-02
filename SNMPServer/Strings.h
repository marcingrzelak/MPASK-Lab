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
#pragma endregion

