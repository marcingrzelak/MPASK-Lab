#include "pch.h"
#include "FileHandler.h"


FileHandler::FileHandler(string filePath)
{
	file.open(filePath, ios::in);
}


FileHandler::~FileHandler()
{
	file.close();
}

void FileHandler::FileRead()
{
	string line;
	getline(file, line);
	cout << line << endl;
}


