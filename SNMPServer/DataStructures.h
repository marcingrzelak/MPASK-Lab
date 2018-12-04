#pragma once

class DataStructures
{
public:
	DataStructures();
	~DataStructures();
};

class ImportsTemp
{
public:
	string imports;
	string fileName;

	//indexes from regex match
	int iImports = 1, iFileName = 2;
};

class Imports
{
public:
	vector <string> imports;
};

class ObjectIdentifier
{
public:
	string name;
	string parent;
	int oid;

	//indexes from regex match
	int iName = 1, iParent = 2, iOid = 3;
};

class ObjectIdentifierLongParent
{
public:
	//indexes from regex match
	int iName = 1, iOid = 3;
};

class ObjectType
{
public:
	string name;
	string syntax;
	string access;
	string status;
	string description;
	string parent;
	int oid;

	//indexes from regex match
	int iName = 1, iSyntax = 2, iAccess = 3, iStatus = 4, iDescription = 5, iIndex = 6, iParent = 7, iOid = 8;
};

class SpecialDataType
{
public:
	string name;
	int size = -1;
	long long sizeMin = -1;
	long long sizeMax = -1;

	//indexes from regex match
	int iSize = 1, iSizeMin = 2, iSizeMax = 3;
};

class IndexTemp
{
public:
	string indexes;
	string indexesClean;
};

class Index
{
public:
	string name;
	vector <string> indexes;

	static void showIndexes(vector<Index> pVIndex);
};

class DataType
{
public:
	string name;
	string visibility;
	unsigned long typeID = ULONG_MAX;
	string keyword;
	string type;
	int size = -1;
	long long sizeMin = -1;
	long long sizeMax = -1;

	//indexes from regex match
	int iName = 1, iVisibility = 2, iTypeID = 3, iKeyword = 4, iType = 5, iSize = 9, iSizeMin = 11, iSizeMax = 12;

	static void showDataTypes(vector<DataType> pVDataType);
};

class ChoiceTemp
{
public:
	string name;
	string types;

	//indexes from regex match
	int iName = 1, iTypes = 2;
};

class Choice
{
public:
	string name;
	vector <string> typeName;
	vector <string> type;

	//indexes from regex match
	int iTypeName = 1, iType = 2;

	static void showChoices(vector<Choice> pVChoice);
};

class SequenceTemp
{
public:
	string name;
	string types;

	//indexes from regex match
	int iName = 1, iTypes = 2;
};

class Sequence
{
public:
	string name;
	vector <string> typeName;
	vector <string> type;

	//indexes from regex match
	int iTypeName = 1, iType = 2;

	static void showSequences(vector<Sequence> pVSequence);
};