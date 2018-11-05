#pragma once

class FileHandler
{
public:
	FileHandler(string path);
	~FileHandler();

	string FileRead();
	string FileGetLine();

	fstream file;
};

