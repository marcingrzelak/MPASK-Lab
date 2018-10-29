#pragma once

class FileHandler
{
public:
	FileHandler(string);
	~FileHandler();

	void FileRead();
	string FileGetLine();


	fstream file;
};

