#include "pch.h"
#include "CheckValue.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Regex.h"
#include "Strings.h"
#include "Exceptions.h"


CheckValue::CheckValue()
{
}


CheckValue::~CheckValue()
{
}


void CheckValue::clearValues()
{
	isValueNumber = false;
	isObjectIdentifier = false;
	pValueINT = LLONG_MIN;
	byteCount = 0;
	bitCount = 0;
	type = 0;
	typeDataType = 0;
	typeSequence = 0;
	indexDataType = -1;
	indexSequence = -1;
	sequenceValues.clear();
	objectIdentifierSubidentifiers.clear();
	sequenceDefaultTypes.clear();
	sequenceDataTypeIndexes.clear();
	sequenceTypeID.clear();
	sequenceBytesCount.clear();
	sequenceKeywords.clear();
	sequenceVisibilities.clear();
}

void CheckValue::setValueParameters(string pValue)
{
	checkIsObjectIdentifier(pValue);
	checkIsNumber(pValue);
	lengthCalc(pValue);
}

void CheckValue::checkIsNumber(string pValue)
{
	if (isObjectIdentifier)
	{
		isValueNumber = false;
		pValueINT = LLONG_MIN;
	}
	else
	{
		try
		{
			isValueNumber = true;
			pValueINT = stoll(pValue);
		}
		catch (const std::exception&)
		{
			isValueNumber = false;
			pValueINT = LLONG_MIN;
		}
	}
}

void CheckValue::lengthCalc(string pValue)
{
	if (isValueNumber)
	{
		if (pValueINT < 0)
		{
			bitCount = floor(log2(abs(pValueINT)) + 1);
		}
		else if (pValueINT == 0)
		{
			bitCount = 1;
		}
		else
		{
			bitCount = floor(log2(abs(pValueINT)) + 1) + 1;
		}
		byteCount = ceil(bitCount / 8.0);
	}
	else
	{
		bitCount = 0;
		byteCount = pValue.size();
	}
}

void CheckValue::checkIsObjectIdentifier(string pValue)
{
	string objectTypeValue;
	pValue += ".";
	regex rgx = Regex::objectIdentifierCheckType();
	sregex_iterator objectTypeIterator(pValue.begin(), pValue.end(), rgx), endIterator;
	int i = 0;
	while (objectTypeIterator != endIterator)
	{
		objectTypeValue += (*objectTypeIterator)[0];
		objectIdentifierSubidentifiers.push_back(string((*objectTypeIterator)[0]).substr(0, string((*objectTypeIterator)[0]).size() - 1));
		++objectTypeIterator;
		++i;
	}
	if (pValue == objectTypeValue && i > 2)
	{
		isObjectIdentifier = true;
	}
	else
	{
		objectIdentifierSubidentifiers.clear();
		isObjectIdentifier = false;
	}
}

int CheckValue::checkValueType(string pValue, string pSyntax, vector<DataType> &pVDataType, vector<Sequence> &pVSequence)
{
	type = 0;
	typeDataType = 0;
	typeSequence = 0;

	setValueParameters(pValue);

	type = defaultTypeCheck(pSyntax, isValueNumber);

	if (type < 0)//blad typu danych
	{
		return -1;
	}
	else if (type > 0)//podstawowy typ danych
	{
		return 0;
	}
	else if (type == 0)//niepodstawowy typ danych
	{
		//sprawdzamy czy obiekt jest typu data type
		typeDataType = dataTypeCheck(pValue, pSyntax, pVDataType);
		if (typeDataType < 0)//blad typu danych
		{
			return -1;
		}
		else if (typeDataType > 0)//podstawowy typ danych
		{
			return 0;
		}
		else if (typeDataType == 0)//obiekt nie jest typu data type
		{
			//sprawdzamy czy obiekt jest typu sequence
			typeSequence = sequenceTypeCheck(pSyntax, pVSequence, pVDataType, pValue);
			if (typeSequence < 0)//blad typu danych
			{
				return -1;
			}
			else if (typeSequence > 0)//jest ok
			{
				return 0;
			}
			else if (typeSequence == 0)
			{
				//todo jezeli analizujemy tez choice'y to dodac kolejne sprawdzenie
				return 0;
			}
		}
	}
	return -2;//nie znaleziono zadnego typu
}

int CheckValue::checkValueType(string pValue, string pSyntax)
{
	type = 0;

	setValueParameters(pValue);

	type = defaultTypeCheck(pSyntax, isValueNumber);

	if (type <= 0)//blad typu danych
	{
		return -1;
	}
	else if (type > 0)//podstawowy typ danych
	{
		return 0;
	}
}

short CheckValue::defaultTypeCheck(string pSyntax, bool &isValueNumber)
{
	//sprawdzamy czy podany obiekt ma jeden z domyslnych typow
	smatch result;
	regex_search(pSyntax, result, Regex::dataTypeEncoder());

	if (result[0].matched)
	{
		if (result[1].matched)//integer
		{
			if (!(this->isValueNumber))
			{
				return -1;
			}
			else
			{
				return INTEGER_TAG_NUMBER;
			}
		}
		else if (result[2].matched)//octet string
		{
			isValueNumber = false;
			return OCTET_STRING_TAG_NUMBER;
		}
		else if (result[3].matched)//object identifier
		{
			isValueNumber = false;
			if (isObjectIdentifier)
			{
				return OBJECT_IDENTIFIER_TAG_NUMBER;
			}
			else
			{
				return -2;
			}
		}
		else if (result[4].matched)//null
		{
			return NULL_TAG_NUMBER;
		}
	}
	return 0;
}

short CheckValue::dataTypeCheck(string pValue, string pSyntax, vector<DataType> &pVDataType)
{
	indexDataType = -1;
	//sprawdzamy czy podany obiekt ma typ zawarty w data type
	for (size_t i = 0; i < pVDataType.size(); i++)
	{
		if (pSyntax.find(pVDataType.at(i).name) != string::npos)
		{
			indexDataType = i;
			CheckValue tmp;
			tmp.setValueParameters(pValue);
			short returnedType;
			returnedType = tmp.defaultTypeCheck(pVDataType.at(i).type, isValueNumber);

			if (returnedType <= 0)//blad typu danych
			{
				return -1;
			}
			else//podstawowy typ danych
			{
				return returnedType;
			}
		}
	}
	return 0;//typ nie jest zawarty w data type
}

short CheckValue::sequenceTypeCheck(string pSyntax, vector<Sequence>& pVSequence, vector<DataType>& pVDataType, string pValue)
{
	indexSequence = -1;
	//sprawdzamy czy podany obiekt ma typ zawarty w sequence
	for (size_t i = 0; i < pVSequence.size(); i++)
	{
		if (pSyntax.find(pVSequence.at(i).name) != string::npos)
		{
			indexSequence = i;

			//zapisanie podanych wartosci do wektora
			stringstream test(pValue);
			string segment;
			while (std::getline(test, segment, ','))
			{
				sequenceValues.push_back(segment);
			}

			//czy ilosc wartosci zgadza sie iloscia w sekwencji
			if (sequenceValues.size() != pVSequence.at(i).type.size())
			{
				return -1;
			}

			//dla kazdego elem. sekwencji sprawdzamy typ
			for (size_t j = 0; j < pVSequence.at(i).type.size(); j++)
			{
				CheckValue tmp;
				short returnedType;
				tmp.setValueParameters(sequenceValues.at(j));
				returnedType = tmp.defaultTypeCheck(pVSequence.at(i).type.at(j), tmp.isValueNumber);

				if (returnedType < 0)//blad typu danych
				{
					return -1;
				}
				else if (returnedType > 0)//podstawowy typ danych
				{
					sequenceDefaultTypes.push_back(returnedType);
					sequenceDataTypeIndexes.push_back(-1);
					sequenceTypeID.push_back(NULL);
					sequenceBytesCount.push_back(tmp.byteCount);
					sequenceKeywords.push_back("");
					sequenceVisibilities.push_back(IDENTIFIER_CLASS_UNIVERSAL);
				}
				else if (returnedType == 0)//niepodstawowy typ danych
				{
					//sprawdzamy czy obiekt jest typu data type
					tmp.typeDataType = tmp.dataTypeCheck(sequenceValues.at(j), pVSequence.at(i).type.at(j), pVDataType);
					if (tmp.typeDataType < 0)//blad typu danych
					{
						return -1;
					}
					else if (tmp.typeDataType > 0)//podstawowy typ danych
					{
						sequenceDefaultTypes.push_back(tmp.typeDataType);
						sequenceDataTypeIndexes.push_back(tmp.indexDataType);
						sequenceTypeID.push_back(pVDataType.at(tmp.indexDataType).typeID);
						sequenceBytesCount.push_back(tmp.byteCount);
						sequenceKeywords.push_back(pVDataType.at(tmp.indexDataType).keyword);
						sequenceVisibilities.push_back(pVDataType.at(tmp.indexDataType).visibility);
					}
					else if (tmp.typeDataType == 0)//obiekt nie jest typu data type
					{
						//todo tu powinnismy sprawdzac kolejny raz sekwencje (rekurencja)??
						return -2;//nie znaleziono zadnego typu
					}
				}
				//return -2;//nie znaleziono zadnego typu
			}
			return 1;
		}
	}
	return 0;//typ nie jest zawarty w sequence
}

int CheckValue::checkValueSize(string pName, vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence)
{
	short oTSCreturned = objectTypeSizeCheck(pVObjectTypeSize, pName);
	if (oTSCreturned == 0)//rozmiar ok
	{
		return 0;
	}
	else if (oTSCreturned > 0)//brak ograniczen w objectTypeSize
	{
		if (typeDataType == 0 && typeSequence == 0)//podstawowy typ, dodac potem sprawdzanie typeChoice itp todo
		{
			//sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego
			short dSCreturned = defaultSizeCheck(type);
			return dSCreturned;
		}
		else if (typeDataType > 0)//typ w data type
		{
			//sprawdzamy ograniczenia zawarte w data type
			short dTSCretuened = dataTypeSizeCheck(pVDataType);
			if (dTSCretuened == 0)//rozmiar ok
			{
				return 0;
			}
			else if (dTSCretuened > 0)//brak ograniczen w DataType
			{
				//sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego
				short dSCreturned = defaultSizeCheck(type);
				return dSCreturned;
			}
			else//blad rozmiaru
			{
				return -1;
			}
		}
		else if (typeSequence > 0)
		{
			//sprawdzamy ograniczenia zawarte w sequence dla kazdego elem
			short sSCreturned = sequenceSizeCheck(pVObjectTypeSize, pVDataType, pVSequence);
			if (sSCreturned == 0)//rozmiary ok
			{
				return 0;
			}
			else//blad rozmiaru
			{
				return -1;
			}
		}
		else
		{
			//todo kolejne else if do sprawdzania choice'ow itp
			return 0;
		}
	}
	else//blad
	{
		return -1;
	}
}

int CheckValue::checkValueSize()
{
	short dSCreturned = defaultSizeCheck(type);
	return dSCreturned;
}

short CheckValue::objectTypeSizeCheck(vector<ObjectTypeSize> pVObjectTypeSize, string pName)
{
	//dla danej nazwy obiektu np. sysName
	//sprawdzamy ograniczenia rozmiarow podane w deklaracji obiektu
	//te ograniczenia maja wyzszy priorytet niz ograniczenia podane w deklaracji typu
	//0 rozmiar ok
	//<0 rozmiar poza zakresem lub brak definicji dopuszczalnego rozmiaru
	//>0 nie znaleziono ograniczen
	for (size_t i = 0; i < pVObjectTypeSize.size(); i++)
	{
		if (pVObjectTypeSize.at(i).name == pName)
		{
			short cSreturned = checkSize(pVObjectTypeSize.at(i).size, pVObjectTypeSize.at(i).sizeMin, pVObjectTypeSize.at(i).sizeMax);
			return cSreturned;
		}
	}
	return 1;
}

short CheckValue::defaultSizeCheck(int pType)
{
	short cSreturned;
	if (type == INTEGER_TAG_NUMBER)
	{
		cSreturned = checkSize(-1, INT_MIN, INT_MAX);
	}
	else if (type == OCTET_STRING_TAG_NUMBER)
	{
		cSreturned = 0;
	}
	else if (type == NULL_TAG_NUMBER)
	{
		cSreturned = 0;
	}
	else if (type == OBJECT_IDENTIFIER_TAG_NUMBER)
	{
		cSreturned = 0;
	}
	else if (type == SEQUENCE_TAG_NUMBER)
	{
		cSreturned = 0;
	}

	return cSreturned;
}

short CheckValue::dataTypeSizeCheck(vector<DataType> &pVDataType)
{
	//nie trzeba sprawdzac indexDataType bo zawsze bedzie >= 0 w tym miejscu
	short returnedValue = checkSize(pVDataType.at(indexDataType).size, pVDataType.at(indexDataType).sizeMin, pVDataType.at(indexDataType).sizeMax);
	return returnedValue;
}

short CheckValue::sequenceSizeCheck(vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence)
{
	for (size_t i = 0; i < pVSequence.at(indexSequence).type.size(); i++)
	{
		CheckValue tmp;
		tmp.setValueParameters(sequenceValues.at(i));

		if (sequenceDataTypeIndexes.at(i) == -1)//typ podst.
		{
			tmp.type = sequenceDefaultTypes.at(i);
		}
		else
		{
			tmp.indexDataType = sequenceDataTypeIndexes.at(i);
			tmp.typeDataType = sequenceDefaultTypes.at(i);
			tmp.type = sequenceDefaultTypes.at(i);
		}

		int returnedValue = tmp.checkValueSize("", pVObjectTypeSize, pVDataType, pVSequence);

		if (returnedValue < 0)
		{
			return -1;
		}
	}
	return 0;
}

short CheckValue::checkSize(int pSize, long long pSizeMin, long long pSizeMax)
{
	if (pSize != -1)
	{
		if ((this->byteCount) > pSize)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else if (pSizeMin != -1 && pSizeMax != -1)
	{
		//todo zmienic regexa brak rozroznienia SIZE(x..y) od (x..y)
		if ((this->isValueNumber))
		{
			if ((this->pValueINT) < pSizeMin || (this->pValueINT) > pSizeMax)
			{
				return -2;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//brak ograniczen
		return 1;
	}
}

short CheckValue::checkValue(string pValue, TreeNode *pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	clearValues();
	int cVTreturned = checkValueType(pValue, pNode->syntax, pVDataType, pVSequence);
	if (cVTreturned == 0)//typ ok
	{
		int cVSreturned = checkValueSize(pNode->name, pVObjectTypeSize, pVDataType, pVSequence);
		if (cVSreturned == 0)//rozmiar ok
		{
			return 0;
		}
		else
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}
}