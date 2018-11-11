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

	int nameObjectType = 1, syntaxObjectType = 2, accessObjectType = 3, statusObjectType = 4, descriptionObjectType = 5, oidTextObjectType = 6, oidObjectType = 7;
	int idObjectIdentifier = 1, parentObjectIdentifier = 9, oidObjectIdentifier = 10;
	int parentObjectIdentifier2 = 4, parent2ObjectIdentifier = 5, oidparent2 = 6, parent3ObjectIdentifier = 7, oidparent3 = 8;
	int im1Imports = 1, im2Imports = 2, im3Imports = 3, im4Imports = 4, im5Imports = 5, im6Imports = 6, file1Imports = 7;
	int nameDataType = 1, visabilityDataType = 2, typeidDataType = 3, keywordDataType = 4, type = 5, sizeDataType = 9, sizeMinDataType = 11, sizeMaxDataType = 12;

	rawFile = mib.FileRead();
	cleanedFile = regex_replace(rawFile, rgx.whitespaces(), " ");

	//regex IMPORTS("(BEGIN\\s*IMPORTS\\s*((.*?),\\s*)*(.*?)\\s*FROM\\s*(\\S*).*;)"); //powinno dzialac niezaleznie od ilosci importow
	regex IMPORTS("BEGIN\\s*IMPORTS\\s*(.*?),\\s*(.*?),\\s*(.*?),\\s*(.*?),\\s*(.*?),\\s*(.*?)\\s*FROM\\s*(\\S*).*;");
	regex OBJECT_TYPE("(\\w*)\\s*OBJECT-TYPE\\s*SYNTAX\\s*(.*?)ACCESS\\s*(.*?)STATUS\\s*(.*?)DESCRIPTION\\s*(.*?)::=\\s*.\\s*(\\w*)\\s*(\\d)\\s*.");
	regex DATA_TYPE("(\\w*)\\s*::=\\s*.(UNIVERSAL|APPLICATION|CONTEXT-SPECIFIC|PRIVATE)\\s*(\\d*).\\s*(IMPLICIT|EXPLICIT)\\s*(INTEGER|OCTET STRING|OBJECT IDENTIFIER|NULL)\\s*.(((SIZE)\\s*.(\\d*).)|((\\d*).{2}(\\d*))).");
	regex OBJECT_IDENTIFIER("\\s*(\\S*)\\s*OBJECT\\s*IDENTIFIER\\s*::=\\s*.\\s*(((\\S*)\\s*(\\S*)\\((\\d*)\\)\\s*(\\S*)\\((\\d*)\\))|(\\S*))\\s*(\\d*)\\s*.");

	sregex_iterator iter(cleanedFile.begin(), cleanedFile.end(), OBJECT_IDENTIFIER);
	sregex_iterator end;

	while (iter != end)
	{
	//	sObjectType.name = (*iter)[name];
	//	sObjectType.syntax = (*iter)[syntax];
	//	sObjectType.access = (*iter)[access];
	//	sObjectType.status = (*iter)[status];
	//	sObjectType.description = (*iter)[description];
	//	sObjectType.oidText = (*iter)[oidText];
	//	sObjectType.oid = stoi((string)(*iter)[oid]);
	//	vObjectType.push_back(sObjectType);

	//	cout << sObjectType.name << "\t\t\t";
	//	cout << sObjectType.syntax << "\t";
	//	cout << sObjectType.access << "\t";
	//	cout << sObjectType.status << "\t";
	//	cout << sObjectType.oidText << "\t";
	//	cout << sObjectType.oid << endl;
		++iter;
	}

	sregex_iterator iter2(cleanedFile.begin(), cleanedFile.end(), IMPORTS);
	sregex_iterator end2;

	while (iter2 != end2)
	{
		cout << (*iter2)[3] << endl;;
		++iter2;
		cout << "Znalazlem 2" << endl;
	}

	//kazdy ciag do osobnego elementu wektora
	//istringstream iss(cleanedFile);
	//vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
	return(0);
}