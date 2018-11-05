#pragma once

class FileHandler
{
public:
	FileHandler();
	~FileHandler();

	string FileRead(string);
	string FileGetLine();

	ifstream ifs;
};

