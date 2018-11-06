#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"

int main()
{
	FileHandler mib("MIBS/RFC-1213-MIB.txt");
	Regex rgx;
	string rawFile, cleanedFile;
	vector <ObjectType> vObjectType;
	ObjectType sObjectType;

	int name = 2, syntax = 8, access = 11, status = 14, description = 17, oidText = 22, oid = 24;

	rawFile = mib.FileRead();
	cleanedFile = regex_replace(rawFile, rgx.whitespaces(), " ");

	regex OBJECT_TYPE("((\\w*)(\\s*)(OBJECT-TYPE)(\\s*)(SYNTAX)(.*)(ACCESS)(\\s*)(.*)(STATUS)(\\s*)(.*)(DESCRIPTION)(\\s*)(.*)(::=)(\\s*)(.)(\\s*)(\\w*)(\\s*)(\\d)(\\s*)(.))");

	sregex_iterator iter(cleanedFile.begin(), cleanedFile.end(), OBJECT_TYPE);
	sregex_iterator end;

	while (iter != end)
	{
		sObjectType.name = (*iter)[name];
		sObjectType.syntax = (*iter)[syntax];
		sObjectType.access = (*iter)[access];
		sObjectType.status = (*iter)[status];
		sObjectType.description = (*iter)[description];
		sObjectType.oidText = (*iter)[oidText];
		sObjectType.oid = stoi((string)(*iter)[oid]);
		vObjectType.push_back(sObjectType);
		++iter;
	}

	//kazdy ciag do osobnego elementu wektora
	//istringstream iss(cleanedFile);
	//vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
	return(0);
}