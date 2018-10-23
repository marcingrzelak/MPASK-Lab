#pragma once

class FileHandler
{
public:
	FileHandler(string);
	~FileHandler();

	void FileRead();


	fstream file;
};

