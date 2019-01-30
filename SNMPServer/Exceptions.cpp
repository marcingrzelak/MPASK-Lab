#include "pch.h"
#include "Exceptions.h"
#include "Strings.h"

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
