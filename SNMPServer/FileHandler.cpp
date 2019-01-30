#include "pch.h"
#include "FileHandler.h"
#include "Regex.h"
#include "Exceptions.h"

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
	if (file.good() == false)
	{
		throw eFileOpen();
	}

	string wholeFile = "", line;
	while (!file.eof())
	{
		getline(file, line);		
		line = regex_replace(line, Regex::comments(), " ");
		wholeFile.append(line+" ");
	}

	cout << "\tKomentarze usuniete" << endl;

	wholeFile = regex_replace(wholeFile, Regex::whitespaces(), " ");
	cout << "\tBiale znaki usuniete" << endl;

	return wholeFile;
}