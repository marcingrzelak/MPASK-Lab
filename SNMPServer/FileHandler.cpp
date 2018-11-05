#include "pch.h"
#include "FileHandler.h"


FileHandler::FileHandler()
{
	
}


FileHandler::~FileHandler()
{
	ifs.close();
}

string FileHandler::FileRead(string filePath)
{
	ifstream ifs(filePath.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(bytes.data(), fileSize);

	return string(bytes.data(), fileSize);
}

string FileHandler::FileGetLine()
{
	string line;
	//dodac zabezpieczenie konca pliku

	//getline(file, line);
	return line;
}