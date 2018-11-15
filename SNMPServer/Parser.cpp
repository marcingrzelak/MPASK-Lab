#include "pch.h"
#include "Parser.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"


Parser::Parser()
{
}


Parser::~Parser()
{
}

void Parser::wholeFileParse(string pFilePath, Tree pOIDTree, vector<DataType> &pVDataType)
{
	FileHandler file(pFilePath);
	Regex Rgx;
	regex rgx;
	string mainFile, importFilePath, importFile, import1;
	smatch result;

	ObjectType sObjectType;
	ObjectIdentifier sObjectIdentifier;
	ObjectIdentifierExtended sObjectIdentifierExtended;
	DataType sDataType;
	Imports2 sImports2;
	vector<Imports2> vImports2;
	Imports3 sImports3;
	vector<Imports3> vImports3;

	sregex_iterator endIterator;

	cout << "Przygotowywanie pliku " << pFilePath << " do parsowania" << endl;
	mainFile = file.FileRead();
	cout << "Przygotowywanie zakonczone" << endl;

	rgx = Rgx.IMPORTS1(); //sekcja z importami
	regex_search(mainFile, result, rgx);
	import1 = result[1];

	rgx = Rgx.IMPORTS2(); //ciag elem do importu z pojedynczego pliku
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

	for (unsigned int i = 0; i < vImports2.size(); i++) //iteracja po wszystkich plikach
	{
		rgx = Rgx.IMPORTS3(); //konkretne elem. do importu z danego pliku
		sregex_iterator import3Iterator(vImports2.at(i).imports.begin(), vImports2.at(i).imports.end(), rgx);

		while (import3Iterator != endIterator)
		{
			sImports3.imports.push_back((*import3Iterator)[1]); //tutaj mamy konkretne elem. do importu. todo: import tylko tych, teraz importujemy wszystko
			++import3Iterator;
		}

		importFilePath.append("MIBS/");
		importFilePath.append(vImports2.at(i).fileName);
		importFilePath.append(".txt");

		wholeFileParse(importFilePath, pOIDTree, pVDataType); //rekurencyjne odpalenie pliku z importami do parsownia
	}

	//import OBJECT IDENTIFIER - nowe OIDy
	rgx = Rgx.OBJECT_IDENTIFIER();
	sregex_iterator objectIdentifierIterator(mainFile.begin(), mainFile.end(), rgx);
	while (objectIdentifierIterator != endIterator)
	{
		if (string((*objectIdentifierIterator)[sObjectIdentifier.iParent]).size() != 0) //skladnia: directory OBJECT IDENTIFIER ::= { internet 1 }
		{
			sObjectIdentifier.name = (*objectIdentifierIterator)[sObjectIdentifier.iName];
			sObjectIdentifier.parent = (*objectIdentifierIterator)[sObjectIdentifier.iParent];
			try
			{
				sObjectIdentifier.oid = stoi((*objectIdentifierIterator)[sObjectIdentifier.iOid]);
			}
			catch (const std::exception&)
			{

			}

			TreeNode* parent = pOIDTree.findNode(sObjectIdentifier.parent, pOIDTree.root);
			pOIDTree.addNode(sObjectIdentifier.name, sObjectIdentifier.oid, parent);
			cout << "Dodano OID: " << sObjectIdentifier.name << endl;

		}
		else // skladnia: internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
		{
			sObjectIdentifierExtended.name = (*objectIdentifierIterator)[sObjectIdentifierExtended.iName];
			sObjectIdentifierExtended.parent = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent];
			sObjectIdentifierExtended.parent2 = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent2];
			sObjectIdentifierExtended.parent3 = (*objectIdentifierIterator)[sObjectIdentifierExtended.iParent3];
			try
			{
				sObjectIdentifierExtended.oidParent2 = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOidParent2]);
				sObjectIdentifierExtended.oidParent3 = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOidParent3]);
				sObjectIdentifierExtended.oid = stoi((*objectIdentifierIterator)[sObjectIdentifierExtended.iOid]);
			}
			catch (const std::exception&)
			{

			}

			TreeNode* node = pOIDTree.findNode(sObjectIdentifierExtended.parent, pOIDTree.root);
			TreeNode* node2 = pOIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent2, sObjectIdentifierExtended.oidParent2, node);
			TreeNode* node3 = pOIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent3, sObjectIdentifierExtended.oidParent3, node2);

			pOIDTree.addNode(sObjectIdentifierExtended.name, sObjectIdentifierExtended.oid, node3);
			cout << "Dodano OID: " << sObjectIdentifierExtended.name << endl;
		}
		++objectIdentifierIterator;
	}

	cout << "Zaimportowano deklaracje OBJECT IDENTIFIER z pliku " << pFilePath << endl << endl;

	//import OBJECT-TYPE - nowe obiekty zarz¹dzalne
	rgx = Rgx.OBJECT_TYPE();
	sregex_iterator objectTypeIterator(mainFile.begin(), mainFile.end(), rgx);
	while (objectTypeIterator != endIterator)
	{
		sObjectType.name = (*objectTypeIterator)[sObjectType.iName];
		sObjectType.syntax = (*objectTypeIterator)[sObjectType.iSyntax];
		sObjectType.access = (*objectTypeIterator)[sObjectType.iAccess];
		sObjectType.status = (*objectTypeIterator)[sObjectType.iStatus];
		sObjectType.description = (*objectTypeIterator)[sObjectType.iDescription];
		sObjectType.parent = (*objectTypeIterator)[sObjectType.iParent];
		try
		{
			sObjectType.oid = stoi((*objectTypeIterator)[sObjectType.iOid]);
		}
		catch (const std::exception&)
		{

		}

		TreeNode* parent = pOIDTree.findNode(sObjectType.parent, pOIDTree.root);
		pOIDTree.addNodeObject(sObjectType.name, sObjectType.oid, parent, sObjectType.syntax, sObjectType.access, sObjectType.description);
		cout << "Dodano obiekt: " << sObjectType.name << endl;

		++objectTypeIterator;
	}

	cout << "Zaimportowano deklaracje OBJECT-TYPE z pliku " << pFilePath << endl << endl;

	//import data type - nowe typy danych
	rgx = Rgx.DATA_TYPE();
	sregex_iterator dataTypeIterator(mainFile.begin(), mainFile.end(), rgx);
	while (dataTypeIterator != endIterator)
	{
		sDataType.name = (*dataTypeIterator)[sDataType.iName];
		sDataType.visibility = (*dataTypeIterator)[sDataType.iVisibility];
		sDataType.typeID = (*dataTypeIterator)[sDataType.iTypeID];
		sDataType.keyword = (*dataTypeIterator)[sDataType.iKeyword];
		sDataType.type = (*dataTypeIterator)[sDataType.iType];

		if (string((*dataTypeIterator)[sDataType.iSize]).size() != 0) //skladnia: IpAddress ::= [APPLICATION 0] IMPLICIT OCTET STRING (SIZE (4))
		{
			try
			{
				sDataType.size = stoi((*dataTypeIterator)[sDataType.iSize]);
			}
			catch (const std::exception&)
			{
				sDataType.size = -1;
			}

			sDataType.sizeMin = -1;
			sDataType.sizeMax = -1;

		}
		else //skladnia: Counter ::= [APPLICATION 1] IMPLICIT INTEGER (0..4294967295)
		{
			try
			{
				sDataType.sizeMin = stoll((*dataTypeIterator)[sDataType.iSizeMin]);
				sDataType.sizeMax = stoll((*dataTypeIterator)[sDataType.iSizeMax]);
			}
			catch (const std::exception&)
			{
				sDataType.sizeMin = -1;
				sDataType.sizeMax = -1;
			}

			sDataType.size = -1;

		}
		pVDataType.push_back(sDataType);
		++dataTypeIterator;
		cout << "Dodano typ danych: " << sDataType.name << endl;
	}

	cout << "Zaimportowano definicje nowych typow danych z pliku " << pFilePath << endl << endl;
	cout << "Koniec pliku " << pFilePath << endl << endl;
	return;
}