#include "pch.h"
#include "FileHandler.h"
#include "Regex.h"


FileHandler::FileHandler(string path)
{
	file.open(path, ios::in);
}

FileHandler::~FileHandler()
{
	file.close();
}

string FileHandler::FileRead()
{
	string wholeFile = "", line;
	while (!file.eof())
	{
		getline(file, line);		
		line = regex_replace(line, Regex::comments(), " ");
		wholeFile.append(line+" ");
	}

	wholeFile = regex_replace(wholeFile, Regex::whitespaces(), " ");

	return wholeFile;
}