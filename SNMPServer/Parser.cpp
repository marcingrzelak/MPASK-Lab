#include "pch.h"
#include "Parser.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"
#include "Exceptions.h"


Parser::Parser()
{
}


Parser::~Parser()
{
}

void Parser::wholeFileParse(string pFilePath, Tree pOIDTree, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	FileHandler file(pFilePath);
	regex rgx, rgx2;
	string mainFile, importsFilePath = "MIBS/", importsGeneral, indexes;
	smatch result;

	ImportsTemp sImportsTemp;
	vector<ImportsTemp> vImportsTemp;
	Imports sImports;
	vector<Imports> vImports;
	ObjectIdentifier sObjectIdentifier;
	ObjectIdentifierLongParent sObjectIdentifierLongParent;
	IndexTemp sIndexTemp;
	Index sIndex;
	ObjectType sObjectType;
	DataType sDataType;
	ChoiceTemp sChoiceTemp;
	vector<ChoiceTemp> vChoiceTemp;
	Choice sChoice;
	SequenceTemp sSequenceTemp;
	vector<SequenceTemp> vSequenceTemp;
	Sequence sSequence;
	ObjectTypeSize sObjectTypeSize;

	sregex_iterator endIterator;

	cout << "Odczyt pliku " << pFilePath << " do pamieci" << endl;
	try
	{
		mainFile = file.FileRead();
	}
	catch (Exceptions &e)
	{
		e.message();
		throw eParser();
		return;
	}
	cout << "Odczyt zakonczony" << endl << endl;

#pragma region IMPORTS
	//wyszukanie sekcji z importami
	regex_search(mainFile, result, Regex::importsGeneral());
	importsGeneral = result[1];

	//podzial na ciag elementow do zaimportowania i nazwe pliku
	try
	{
		rgx = Regex::imports();
		sregex_iterator import1Iterator(importsGeneral.begin(), importsGeneral.end(), rgx);
		while (import1Iterator != endIterator)
		{
			sImportsTemp.imports = (*import1Iterator)[sImportsTemp.iImports];
			sImportsTemp.imports.append(",");
			sImportsTemp.fileName = (*import1Iterator)[sImportsTemp.iFileName];
			if ((sImportsTemp.imports != "OBJECT-TYPE,") && (sImportsTemp.fileName != "RFC-1212"))
			{
				vImportsTemp.push_back(sImportsTemp);
			}
			++import1Iterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}

	for (unsigned int i = 0; i < vImportsTemp.size(); i++) //iteracja po wszystkich plikach
	{
		try
		{
			//konkretne elem. do importu z danego pliku
			rgx = Regex::importsOneElement();
			sregex_iterator importsIterator(vImportsTemp.at(i).imports.begin(), vImportsTemp.at(i).imports.end(), rgx);
			while (importsIterator != endIterator)
			{
				sImports.imports.push_back((*importsIterator)[1]); //tutaj mamy konkretne elem. do importu. todo: import tylko tych, teraz importujemy wszystko
				++importsIterator;
			}
		}
		catch (regex_error& e)
		{
			cout << e.what() << endl;
			throw eParser();
		}

		importsFilePath.append(vImportsTemp.at(i).fileName);
		importsFilePath.append(".txt");

		wholeFileParse(importsFilePath, pOIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize); //rekurencyjne odpalenie pliku z importami do parsownia
		importsFilePath.clear();
	}
#pragma endregion importy

#pragma region OBJECT IDENTIFIER
	try
	{
		//wyszukanie sekcji OBJECT-IDENTIFIER
		rgx = Regex::objectIdentifierGeneral();
		TreeNode* parent;
		sregex_iterator objectIdentifierIterator(mainFile.begin(), mainFile.end(), rgx);
		while (objectIdentifierIterator != endIterator)
		{
			sObjectIdentifier.name = (*objectIdentifierIterator)[sObjectIdentifier.iName];
			sObjectIdentifier.parent = (*objectIdentifierIterator)[sObjectIdentifier.iParent];
			try
			{
				sObjectIdentifier.oid = stoi((*objectIdentifierIterator)[sObjectIdentifier.iOid]);
			}
			catch (const std::exception&)
			{
				throw eParserSTOI();
			}

			size_t found = sObjectIdentifier.parent.find(" ");
			if (found == string::npos) //skladnia: directory OBJECT IDENTIFIER ::= { internet 1 }
			{
				parent = pOIDTree.findNode(sObjectIdentifier.parent, pOIDTree.root);
			}
			else // skladnia: internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
			{
				sObjectIdentifier.parent.append(" ");
				string oid;
				//wyszukiwanie kolejnych rodzicow
				rgx2 = Regex::objectIdentifierLongParent();
				sregex_iterator objectIdentifierLongParentIterator(sObjectIdentifier.parent.begin(), sObjectIdentifier.parent.end(), rgx2);
				while (objectIdentifierLongParentIterator != endIterator)
				{
					if ((*objectIdentifierLongParentIterator)[sObjectIdentifierLongParent.iName] == "iso")
					{
						if (string((*objectIdentifierLongParentIterator)[sObjectIdentifierLongParent.iOid]).size() == 0)
						{
							oid.append("1.");
						}
						else
						{
							oid.append((*objectIdentifierLongParentIterator)[sObjectIdentifierLongParent.iOid]);
							oid.append(".");
						}
					}
					else
					{
						oid.append((*objectIdentifierLongParentIterator)[sObjectIdentifierLongParent.iOid]);
						oid.append(".");
					}
					++objectIdentifierLongParentIterator;
				}

				oid = oid.substr(0, oid.size() - 1); //usuniecie ostatniej "." z oid
				try
				{
					parent = pOIDTree.findOID(oid, pOIDTree.root);
				}
				catch (Exceptions &e)
				{
					e.message();
					throw eParser();
				}
			}
			try
			{
				pOIDTree.addNode(sObjectIdentifier.name, sObjectIdentifier.oid, parent);
			}
			catch (Exceptions &e)
			{
				e.message();
				throw eParser();
			}
			cout << "Dodano OID: " << sObjectIdentifier.name << endl;
			++objectIdentifierIterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}	
	cout << "Zaimportowano deklaracje OBJECT IDENTIFIER z pliku " << pFilePath << endl << endl;
#pragma endregion nowe OIDy

#pragma region OBJECT-TYPE
	try
	{
		//wyszukanie sekcji OBJECT-TYPE
		rgx = Regex::objectType();
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
				throw eParserSTOI();
			}

			if (string((*objectTypeIterator)[sObjectType.iIndex]).size() != 0) //jest struktura z indexem
			{
				sIndex.name = sObjectType.name;
				sIndexTemp.indexes = (*objectTypeIterator)[sObjectType.iIndex];

				regex_search(sIndexTemp.indexes, result, Regex::index()); //ciag indexow
				sIndexTemp.indexesClean = result[1];
				sIndexTemp.indexesClean.append(",");

				rgx2 = Regex::indexOneElement(); //poszczegolne nazwy
				sregex_iterator IndexIterator(sIndexTemp.indexesClean.begin(), sIndexTemp.indexesClean.end(), rgx2);
				while (IndexIterator != endIterator)
				{
					sIndex.indexes.push_back((*IndexIterator)[1]);
					++IndexIterator;
				}
				pVIndex.push_back(sIndex);
				sIndex.indexes.clear();
			}

			//sprawdzanie czy jest deklaracja rozmiaru w syntaxie
			regex_search(sObjectType.syntax, result, Regex::sizeGeneral());
			string tmp = result[0];

			if (tmp.size() != 0)
			{
				sObjectTypeSize.name = sObjectType.name;
				regex_search(tmp, result, Regex::size());

				if (result[sObjectTypeSize.iSize].matched) //skladnia rozmiaru: (SIZE (4))
				{
					try
					{
						sObjectTypeSize.size = stoi(result[sObjectTypeSize.iSize]);
					}
					catch (const std::exception&)
					{
						sObjectTypeSize.size = -1;
					}

					sObjectTypeSize.sizeMin = -1;
					sObjectTypeSize.sizeMax = -1;

				}
				else //skladnia rozmiaru: (0..4294967295)
				{
					try
					{
						sObjectTypeSize.sizeMin = stoll(result[sObjectTypeSize.iSizeMin]);
						sObjectTypeSize.sizeMax = stoll(result[sObjectTypeSize.iSizeMax]);
					}
					catch (const std::exception&)
					{
						sObjectTypeSize.sizeMin = -1;
						sObjectTypeSize.sizeMax = -1;
					}

					sObjectTypeSize.size = -1;
				}
				if (sObjectTypeSize.size != -1 || sObjectTypeSize.sizeMin != -1 || sObjectTypeSize.sizeMax != -1)
				{
					pVObjectTypeSize.push_back(sObjectTypeSize);
				}
			}

			TreeNode* parent = pOIDTree.findNode(sObjectType.parent, pOIDTree.root);
			pOIDTree.addNodeObject(sObjectType.name, sObjectType.oid, parent, sObjectType.syntax, sObjectType.access, sObjectType.description);
			cout << "Dodano obiekt: " << sObjectType.name << endl;

			++objectTypeIterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}
	cout << "Zaimportowano deklaracje OBJECT-TYPE z pliku " << pFilePath << endl << endl;
#pragma endregion nowe obiekty zarzadzalne

#pragma region data type
	try
	{
		//wyszukanie sekcji z nowymi typami danych
		rgx = Regex::dataType();
		sregex_iterator dataTypeIterator(mainFile.begin(), mainFile.end(), rgx);
		while (dataTypeIterator != endIterator)
		{
			sDataType.name = (*dataTypeIterator)[sDataType.iName];
			sDataType.visibility = (*dataTypeIterator)[sDataType.iVisibility];
			try
			{
				sDataType.typeID = stoul((*dataTypeIterator)[sDataType.iTypeID]);
			}
			catch (const std::exception&)
			{
				throw eParserSTOI();
			}
			sDataType.keyword = (*dataTypeIterator)[sDataType.iKeyword];
			sDataType.type = (*dataTypeIterator)[sDataType.iType];

			if (string((*dataTypeIterator)[sDataType.iSize]).size() != 0) //skladnia rozmiaru: (SIZE (4))
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
			else //skladnia rozmiaru: (0..4294967295)
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
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}
#pragma endregion nowe typy danych

#pragma region data type choice
	try
	{
		//wyszukanie sekcji z nowymi typami danych typu choice
		rgx = Regex::choiceGeneral();
		sregex_iterator choice1Iterator(mainFile.begin(), mainFile.end(), rgx);
		while (choice1Iterator != endIterator)
		{
			sChoiceTemp.name = (*choice1Iterator)[sChoiceTemp.iName];
			sChoiceTemp.types = (*choice1Iterator)[sChoiceTemp.iTypes];
			sChoiceTemp.types.append(",");
			vChoiceTemp.push_back(sChoiceTemp);
			++choice1Iterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}

	for (unsigned int i = 0; i < vChoiceTemp.size(); i++) //iteracja po wszystkich ciagach typow choice
	{
		try
		{
			rgx = Regex::choiceOneElement();
			sregex_iterator choiceIterator(vChoiceTemp.at(i).types.begin(), vChoiceTemp.at(i).types.end(), rgx);

			while (choiceIterator != endIterator)
			{
				sChoice.typeName.push_back((*choiceIterator)[sChoice.iTypeName]);
				sChoice.type.push_back((*choiceIterator)[sChoice.iType]);

				++choiceIterator;
			}
			sChoice.name = vChoiceTemp.at(i).name;
			pVChoice.push_back(sChoice);
			sChoice.typeName.clear();
			sChoice.type.clear();
		}
		catch (regex_error& e)
		{
			cout << e.what() << endl;
			throw eParser();
		}
	}
#pragma endregion nowe typy danych choice

#pragma region data type sequence
	try
	{
		//wyszukanie sekcji z nowymi typami danych typu sequence
		rgx = Regex::sequenceGeneral();
		sregex_iterator sequence1Iterator(mainFile.begin(), mainFile.end(), rgx);

		while (sequence1Iterator != endIterator)
		{
			sSequenceTemp.name = (*sequence1Iterator)[sSequenceTemp.iName];
			sSequenceTemp.types = (*sequence1Iterator)[sSequenceTemp.iTypes];
			sSequenceTemp.types.append(",");
			vSequenceTemp.push_back(sSequenceTemp);
			++sequence1Iterator;
		}
	}
	catch (regex_error& e)
	{
		cout << e.what() << endl;
		throw eParser();
	}

	for (unsigned int i = 0; i < vSequenceTemp.size(); i++) //iteracja po wszystkich ciagach typow sequence
	{
		try
		{
			rgx = Regex::sequenceOneElement();
			sregex_iterator sequenceIterator(vSequenceTemp.at(i).types.begin(), vSequenceTemp.at(i).types.end(), rgx);

			while (sequenceIterator != endIterator)
			{
				sSequence.typeName.push_back((*sequenceIterator)[sSequence.iTypeName]);
				sSequence.type.push_back((*sequenceIterator)[sSequence.iType]);

				++sequenceIterator;
			}
			sSequence.name = vSequenceTemp.at(i).name;
			pVSequence.push_back(sSequence);
			sSequence.typeName.clear();
			sSequence.type.clear();
		}
		catch (regex_error& e)
		{
			cout << e.what() << endl;
			throw eParser();
		}
	}
#pragma endregion nowe typy danych sequence

	cout << "Zaimportowano definicje nowych typow danych z pliku " << pFilePath << endl << endl;
	cout << "Koniec pliku " << pFilePath << endl << endl;
	return;
}