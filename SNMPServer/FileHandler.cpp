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

string FileHandler::FileGetLine()
{
	string line;
	//dodac zabezpieczenie konca pliku
	getline(file, line);
	return line;
}


