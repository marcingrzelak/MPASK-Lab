#include "pch.h"
#include "Regex.h"


Regex::Regex()
{
}

Regex::~Regex()
{
}

regex Regex::whitespaces()
{
	return regex(R"(\s{2,})");
}

regex Regex::comments()
{
	return regex(R"(-{2}.*)");
}

regex Regex::importsGeneral()
{
	return regex(R"(IMPORTS\s(.*?);)");
}

regex Regex::imports()
{
	return regex(R"((.*?)\sFROM\s(\S*))");
}

regex Regex::importsOneElement()
{
	return regex(R"(\s?(.*?),)");
}

regex Regex::objectIdentifierGeneral()
{
	return regex(R"((\S*)\sOBJECT\sIDENTIFIER\s::=\s\{\s(.*?)\s(\d+)\s\})");
}

regex Regex::objectIdentifierLongParent()
{
	return regex(R"((\S*?)(\((\d+)\))?\s)");
}

regex Regex::objectType()
{
	return regex(R"((\w*)\sOBJECT-TYPE\sSYNTAX\s(.*?)\sACCESS\s(.*?)\sSTATUS\s(.*?)\sDESCRIPTION\s(.*?)\s(INDEX\s\{.*?\}\s)?::=\s\{\s(\S*)\s(\d+)\s\})");
}

regex Regex::sizeGeneral()
{
	return regex(R"(\(.*?\)+)");
}

regex Regex::size()
{
	return regex(R"(\(SIZE\((\d+)\)\)|\((\d+)\.\.(\d+)\))");
}

regex Regex::indexRgx()
{
	return regex(R"(\{\s(.*?)\s\})");
}

regex Regex::indexOneElement()
{
	return regex(R"(\s?(.*?),)");
}

regex Regex::dataType()
{
	return regex(R"((\w*?)\s::=\s?\[?(\S*?)?\s?(\d+)?\]?\s?(\S*?)?\s(INTEGER|OCTET STRING|OBJECT IDENTIFIER|NULL)\s\(?(((SIZE)?\s\((\d+)\))|((\d+)\.{2}(\d+)))?\)?)");
}

regex Regex::dataTypeEncoder()
{
	return regex(R"((INTEGER)|(OCTET STRING)|(OBJECT IDENTIFIER)|(NULL))");
}

regex Regex::choiceGeneral()
{
	return regex(R"((\w*)\s::=\s?CHOICE\s\{\s(.*?)\s\})");
}

regex Regex::choiceOneElement()
{
	return regex(R"(\s?(\S*?)\s(.*?),)");
}

regex Regex::sequenceGeneral()
{
	return regex(R"((\w*)\s::=\sSEQUENCE\s\{(.*?)\s\})");
}

regex Regex::sequenceOneElement()
{
	return regex(R"(\s?(\S*?)\s(.*?),)");
}

regex Regex::objectIdentifierCheckType()
{
	return regex(R"(\d+\.{1})");
}

regex Regex::snmpCommand()
{
	return regex(R"((\w*?)(\s))");
}

regex Regex::snmpGeneral()
{
	return regex(R"((\w*?)\s(\w*?)\s(.*?)\s(.*))");
}

regex Regex::snmpgetOneElement()
{
	return regex(R"((.*?)\s)");
}

regex Regex::snmpsetOneElement()
{
	return regex(R"((.*?)\s(.*?)\s)");
}
