#pragma once
class Regex
{
public:
	Regex();
	~Regex();

	static regex wzorzec();
	static regex whitespaces();
	static regex comments();
};