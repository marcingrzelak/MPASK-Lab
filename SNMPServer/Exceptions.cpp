#include "pch.h"
#include "Exceptions.h"
#include "Strings.h"
#include "ConsoleColor.h"

void eFileOpen::message()
{
	cout << FILE_OPEN_ERR_MSG;
}

void eParser::message()
{
	cout << PARSER_ERR_MSG;
}

void eParserSTOI::message()
{
	cout << PARSER_STOI_ERR_MSG;
}

void eAddNodeParentNull::message()
{
	cout << ADD_NODE_PARENT_NULL_ERR;
}

void eFindOIDParentNull::message()
{
	cout << FIND_OID_PARENT_NULL_ERR;
}

void eFindNodeWordParentNull::message()
{
	cout << FIND_NODE_WORD_PARENT_NULL_ERR;
}

void eAnyValueEncodingWrongType::message()
{
	cout << ENCODING_INPUT_TYPE_ERR;
}

void eEncodingWrongType::message()
{
	cout << ENCODING_TYPE_ERR;
}

void eEncodingWrongSize::message()
{
	cout << ENCODING_SIZE_ERR;
}

void eEncoder::message()
{
	cout << ENCODER_ERR_MSG;
}

void eNodeNotFound::message()
{
	cout << NODE_NOT_FOUND;
}

void eUnknownClass::message()
{
	cout << UNKNOWN_CLASS_ERR;
}

void eUnknownComplexity::message()
{
	cout << UNKNOWN_COMPLEXITY_ERR;
}

void eIdentifier::message()
{
}

void eLength::message()
{
}

void eValue::message()
{
}

void eWrongTag::message()
{
	cout << WRONG_TAG_ERR;
}

void eUnknownTagType::message()
{
	cout << UNKNOWN_TAG_TYPE_ERR;
}

void eLengthKEqual127::message()
{
	cout << LENGTH_K_ERR;
}

void eLengthKGraterThen127::message()
{
	cout << LENGTH_K_ERR;
}

void eLengthEqual128::message()
{
	cout << LENGTH_ERR;
}

void eValueObjectIdentifier::message()
{
	cout << VALUE_OBJECT_IDENTIFIER_ERR;
}

void eDecoder::message()
{
	cout << DECODER_ERR_MSG;
}

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
