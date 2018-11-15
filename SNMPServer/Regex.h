#pragma once
class Regex
{
public:
	Regex();
	~Regex();

	static regex whitespaces();
	static regex comments();
	regex OBJECT_TYPE();
	regex IMPORTS1();
	regex IMPORTS2();
	regex IMPORTS3();
	regex DATA_TYPE();
	regex OBJECT_IDENTIFIER();
	regex SEQUENCE1();
	regex SEQUENCE();
};