#pragma once
class Regex
{
public:
	Regex();
	~Regex();

	static regex whitespaces();
	static regex comments();

	//static const string OBJECT_TYPE_PATTERN = "((\\w*)(\\s*)(OBJECT-TYPE)(\\s*)(SYNTAX)(.*)(ACCESS)(.*)(STATUS)(.*)(DESCRIPTION)(.*)(::=)(\\s*)(.)(\\s*)(\\w*)(\\s*)(\\d)(\\s*)(.))";
};