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
	regex CHOICE();
	regex CHOICE1();
	regex INDEX1();
	regex INDEX2();
	regex OBJECT_IDENTIFIER();
	regex OBJECT_IDENTIFIER1();
	regex SEQUENCE1();
	regex SEQUENCE();
};