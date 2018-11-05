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
	string allFile = "", line;
	while (!file.eof())
	{
		getline(file, line);		
		line = regex_replace(line, Regex::comments(), " ");
		allFile.append(line+" ");
	}

	return allFile;
}

string FileHandler::FileGetLine()
{
	string line;
	//dodac zabezpieczenie konca pliku

	//getline(file, line);
	return line;
}