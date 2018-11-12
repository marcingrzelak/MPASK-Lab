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

	//regex IMPORTS("(BEGIN\\s*IMPORTS\\s*((.*?),\\s*)*(.*?)\\s*FROM\\s*(\\S*).*;)"); //powinno dzialac niezaleznie od ilosci importow
	//regex IMPORTS(R"(BEGIN\s*IMPORTS\s*(.*?),\s*(.*?),\s*(.*?),\s*(.*?),\s*(.*?),\s*(.*?)\s*FROM\s*(\S*).*;)");
};