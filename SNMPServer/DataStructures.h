#pragma once
class DataStructures
{
public:
	DataStructures();
	~DataStructures();
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
	int iName = 1, iSyntax = 2, iAccess = 3, iStatus = 4, iDescription = 5, iParent = 6, iOid = 7;
};

class ObjectIdentifier
{
public:
	string name;
	string parent;
	int oid;

	//indexes from regex match
	int iName = 1, iParent = 9, iOid = 10;
};

class ObjectIdentifierExtended
{
public:
	string name;
	string parent;
	string parent2;
	int oidParent2;
	string parent3;
	int oidParent3;
	int oid;

	//indexes from regex match
	int iName = 1, iParent = 4, iParent2 = 5, iOidParent2 = 6, iParent3 = 7, iOidParent3 = 8, iOid = 10;
};

class DataType
{
public:
	string name;
	string visibility;
	string typeID;
	string keyword;
	string type;
	int size=-1;
	long long sizeMin=-1;
	long long sizeMax=-1;

	//indexes from regex match
	int iName = 1, iVisibility = 2, iTypeID = 3, iKeyword = 4, iType = 5, iSize = 9, iSizeMin = 11, iSizeMax = 12;

	static void showDataTypes(vector<DataType> pVDataType);
};

class Imports2
{
public:
	string imports;
	string fileName;

	int iImports = 1, iFileName = 2;
};

class Imports3
{
public:
	vector <string> imports;
};