#pragma once

class FileHandler
{
public:
	FileHandler(string path);
	~FileHandler();

	string FileRead();
	fstream file;
};

