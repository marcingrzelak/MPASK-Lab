#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"

int main()
{
	FileHandler mib("MIBS/RFC-1213-MIB.txt");
	Regex Rgx;
	regex rgx;
	string mibFile, importFilePath, importFile, import1;
	smatch result;

	ObjectType sObjectType;
	ObjectIdentifier sObjectIdentifier;
	ObjectIdentifierExtended sObjectIdentifierExtended;
	DataType sDataType;
	Imports2 sImports2;
	vector<Imports2> vImports2;
	Imports3 sImports3;
	vector<Imports3> vImports3;

	Tree OIDTree;

	sregex_iterator endIterator;

	mibFile = mib.FileRead();

	rgx = Rgx.IMPORTS1();
	regex_search(mibFile, result, rgx);
	import1 = result[1];

	rgx = Rgx.IMPORTS2();
	sregex_iterator import2Iterator(import1.begin(), import1.end(), rgx);

	while (import2Iterator != endIterator)
	{
		sImports2.imports = (*import2Iterator)[sImports2.iImports];
		sImports2.imports.append(",");
		sImports2.fileName = (*import2Iterator)[sImports2.iFileName];
		if ((sImports2.imports != "OBJECT-TYPE,") && (sImports2.fileName != "RFC-1212"))
		{
			vImports2.push_back(sImports2);
		}
		++import2Iterator;
	}

	for (unsigned int i = 0; i < vImports2.size(); i++)
	{
		rgx = Rgx.IMPORTS3();
		sregex_iterator import3Iterator(vImports2.at(i).imports.begin(), vImports2.at(i).imports.end(), rgx);

		while (import3Iterator != endIterator)
		{
			sImports3.imports.push_back((*import3Iterator)[1]); //tutaj mamy konkretne elem. do importu. teraz importujemy wszystko
			++import3Iterator;
		}

		importFilePath.append("MIBS/");
		importFilePath.append(vImports2.at(i).fileName);
		importFilePath.append(".txt");
		FileHandler importFH(importFilePath);
		importFile = importFH.FileRead();

		//importWholeFile(importFile);
		//import OBJECT IDENTIFIER, OBJECT-TYPE oraz data type z pliku o nazwie importFile
	}

	//import OBJECT IDENTIFIER - nowe OIDy
	rgx = Rgx.OBJECT_IDENTIFIER();
	sregex_iterator objectIdentifierIterator(importFile.begin(), importFile.end(), rgx);
	while (objectIdentifierIterator != endIterator)
	{
		if (string((*objectIdentifierIterator)[sObjectIdentifier.iParent]).size() != 0) //skladnia: directory OBJECT IDENTIFIER ::= { internet 1 }
		{
			sObjectIdentifier.name = (*objectIdentifierIterator)[sObjectIdentifier.iName];
			sObjectIdentifier.parent = (*objectIdentifierIterator)[sObjectIdentifier.iParent];
			sObjectIdentifier.oid = stoi((*objectIdentifierIterator)[sObjectIdentifier.iOid]);

			TreeNode* parent = OIDTree.findNode(sObjectIdentifier.parent, OIDTree.root);
			OIDTree.addNode(sObjectIdentifier.name, sObjectIdentifier.oid, parent);
		}
		else // skladnia: internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
		{
			sObjectIdentifierExtended.name = (*objectIdentifierIterator)[sObjectIdentifierExtended.iName];
			sObjectIdentifierExtended.parent = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent];
			sObjectIdentifierExtended.parent2 = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent2];
			sObjectIdentifierExtended.oidParent2 = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOidParent2]);
			sObjectIdentifierExtended.parent3 = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent3];
			sObjectIdentifierExtended.oidParent3 = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOidParent3]);
			sObjectIdentifierExtended.oid = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOid]);

			TreeNode* node = OIDTree.findNode(sObjectIdentifierExtended.parent, OIDTree.root);
			TreeNode* node2 = OIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent2, sObjectIdentifierExtended.oidParent2, node);
			TreeNode* node3 = OIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent3, sObjectIdentifierExtended.oidParent3, node2);

			OIDTree.addNode(sObjectIdentifierExtended.name, sObjectIdentifierExtended.oid, node3);

		}
		++objectIdentifierIterator;
	}

	//import OBJECT-TYPE - nowe obiekty zarządzalne
	rgx = Rgx.OBJECT_TYPE();
	sregex_iterator objectTypeIterator(importFile.begin(), importFile.end(), rgx);
	while (objectTypeIterator != endIterator)
	{
		sObjectType.name = (*objectTypeIterator)[sObjectType.iName];
		sObjectType.syntax = (*objectTypeIterator)[sObjectType.iSyntax];
		sObjectType.access = (*objectTypeIterator)[sObjectType.iAccess];
		sObjectType.status = (*objectTypeIterator)[sObjectType.iStatus];
		sObjectType.description = (*objectTypeIterator)[sObjectType.iDescription];
		sObjectType.parent = (*objectTypeIterator)[sObjectType.iParent];
		sObjectType.oid = stoi((*objectTypeIterator)[sObjectType.iOid]);

		TreeNode* parent = OIDTree.findNode(sObjectType.parent, OIDTree.root);
		OIDTree.addNodeObject(sObjectType.name, sObjectType.oid, parent, sObjectType.syntax, sObjectType.access, sObjectType.description);
	}

	//import data type - nowe typy danych
	rgx = Rgx.DATA_TYPE();
	sregex_iterator dataTypeIterator(importFile.begin(), importFile.end(), rgx);
	while (dataTypeIterator != endIterator)
	{
		sDataType.name = (*dataTypeIterator)[sDataType.iName];
		sDataType.visibility = (*dataTypeIterator)[sDataType.iVisibility];
		sDataType.typeID = (*dataTypeIterator)[sDataType.iTypeID];
		sDataType.keyword = (*dataTypeIterator)[sDataType.iKeyword];
		sDataType.type = (*dataTypeIterator)[sDataType.iType];

		if (string((*dataTypeIterator)[sDataType.iSize]).size() != 0) //skladnia: IpAddress ::= [APPLICATION 0] IMPLICIT OCTET STRING (SIZE (4))
		{
			sDataType.size = stoi((*dataTypeIterator)[sDataType.iSize]);
		}
		else //skladnia: Counter ::= [APPLICATION 1] IMPLICIT INTEGER (0..4294967295)
		{
			sDataType.sizeMin = stoi((*dataTypeIterator)[sDataType.iSizeMin]);
			sDataType.sizeMax = stoi((*dataTypeIterator)[sDataType.iSizeMax]);
		}
	}

	return(0);
}