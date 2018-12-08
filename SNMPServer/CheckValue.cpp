#include "pch.h"
#include "CheckValue.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Regex.h"
#include "Strings.h"


CheckValue::CheckValue()
{
}


CheckValue::~CheckValue()
{
}


void CheckValue::setValueParameters(string pValue)
{
	checkIsNumber(pValue);
	lengthCalc(pValue);
}

void CheckValue::checkIsNumber(string pValue)
{
	try
	{
		isValueNumber = true;
		pValueINT = stoll(pValue);
	}
	catch (const std::exception&)
	{
		isValueNumber = false;
	}
}

void CheckValue::lengthCalc(string pValue)
{
	if (isValueNumber)
	{
		bitCount = floor(log2(abs(pValueINT)) + 1) + 1;
		byteCount = ceil(bitCount / 8.0);
	}
	else
	{
		byteCount = pValue.size();
	}
}

int CheckValue::checkValueType(string pValue, TreeNode* pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	type = 0;
	typeDataType = 0;
	typeSequence = 0;

	type = defaultTypeCheck(pNode->syntax, isValueNumber);
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
		typeDataType = dataTypeCheck(pNode->syntax, pVDataType);
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
			typeSequence = sequenceTypeCheck(pNode->syntax, pVSequence, pVDataType, pValue);
			if (typeSequence < 0)//blad typu danych
			{
				return -1;
			}
			else if (typeSequence > 0)
			{
				return 0;
			}
			else if (typeSequence == 0)
			{
				//todo jezeli analizujemy tez choice'y to dodac
				return 0;
			}
		}
	}
	return -2;//nie znaleziono zadnego typu
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
			if (!isValueNumber)
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
			//todo sprawdzenie poprawnosci
			isValueNumber = false;
			return OBJECT_IDENTIFIER_TAG_NUMBER;
		}
		else if (result[4].matched)//null
		{
			if (byteCount > 0)
			{
				return -2;
			}
			else
			{
				return NULL_TAG_NUMBER;
			}
		}
	}
	return 0;
}

short CheckValue::dataTypeCheck(string pSyntax, vector<DataType> &pVDataType)
{
	indexDataType = -1;
	//sprawdzamy czy podany obiekt ma typ zawarty w data type
	for (size_t i = 0; i < pVDataType.size(); i++)
	{
		if (pSyntax.find(pVDataType.at(i).name) != string::npos)
		{
			indexDataType = i;
			CheckValue tmp;
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

			//mamy dwie opcje
			//1. SYNTAX  IpRouteEntry
			//2. SYNTAX  SEQUENCE OF IpRouteEntry
			//gdzie IpRouteEntry jest sekwencja
			//2. póki co pomijamy bo to typ tablicowy todo

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
					//todo zapisac returnedType
				}
				else if (type == 0)//niepodstawowy typ danych
				{
					//sprawdzamy czy obiekt jest typu data type
					tmp.typeDataType = tmp.dataTypeCheck(pVSequence.at(i).type.at(j), pVDataType);
					if (tmp.typeDataType < 0)//blad typu danych
					{
						return -1;
					}
					else if (tmp.typeDataType > 0)//podstawowy typ danych
					{
						//todo zapisac tmp.indexDataType oraz tmp.typeDataType
					}
					else if (tmp.typeDataType == 0)//obiekt nie jest typu data type
					{
						//todo tu powinnismy sprawdzac kolejny raz sekwencje (rekurancja)??
						return -2;//nie znaleziono zadnego typu
					}
				}
				return -2;//nie znaleziono zadnego typu
			}
			return 1;
		}
	}
	return 0;//typ nie jest zawarty w sequence
}

int CheckValue::checkValueSize(string pValue, string pName, vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence)
{
	short oTSCreturned = objectTypeSizeCheck(pVObjectTypeSize, pName);//pNode->name
	if (oTSCreturned == 0)//rozmiar ok
	{
		return 0;
	}
	else if (oTSCreturned > 0)//brak ograniczen w objectTypeSize
	{
		if (typeDataType == 0 && typeSequence == 0)//podstawowy typ, dodac potem sprawdzanie typeChoice itp todo
		{
			//todo sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego (zmienna type)
			return 0;
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
				//todo sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego (zmienna typeDataType)
				return 0;
			}
			else if (dTSCretuened < 0)//blad rozmiaru
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
			else if (sSCreturned > 0)//brak ograniczen
			{

			}
			else if (sSCreturned < 0)//blad rozmiaru
			{
				return -1;
			}

		}
		//todo kolejne else if do sprawdzania choice'ow itp
	}
	else if (oTSCreturned < 0)//blad
	{
		return -1;
	}
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
			checkSize(pVObjectTypeSize.at(i).size, pVObjectTypeSize.at(i).sizeMin, pVObjectTypeSize.at(i).sizeMax);
		}
	}
	return 1;
}

short CheckValue::dataTypeSizeCheck(vector<DataType> &pVDataType)
{
	//nie trzeba sprawdzac indexDataType bo zawsze bedzie >= 0 w tym miejscu
	short returnedValue = checkSize(pVDataType.at(indexDataType).size, pVDataType.at(indexDataType).sizeMax, pVDataType.at(indexDataType).sizeMax);
	return returnedValue;
}

short CheckValue::sequenceSizeCheck(vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType, vector<Sequence> &pVSequence)
{
	for (size_t i = 0; i < pVSequence.at(indexSequence).type.size(); i++)
	{
		CheckValue tmp;
		tmp.setValueParameters(sequenceValues.at(i));

		//todo sprawdzic czy pVSequence.at(indexSequence).type.at(i) jest typu podst. czy data type
		//jezeli typ podst to nic nie robic
		//jezeli data type to ustawic tmp.indexDataType oraz tmp.typeDataType na podstawie wartosci zapisanych w sequenceTypeCheck

		int returnedValue = tmp.checkValueSize(sequenceValues.at(i), "", pVObjectTypeSize, pVDataType, pVSequence);

		if (returnedValue < 0)
		{
			return -1;
		}
	}
}

short CheckValue::checkSize(int pSize, long long pSizeMin, long long pSizeMax)
{
	if (pSize != -1)
	{
		if (byteCount > pSize)
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
		//todo co chodzi ze sprawdzeniem isValueNumber
		if (isValueNumber)
		{
			if (byteCount < pSizeMin || byteCount > pSizeMax)
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
			//todo jezeli inna wartosc niz integer moze miec ograniczenia rozmiaru x..y to zmienic
			return 0;
		}

	}
	else
	{
		//brak ograniczen
		return 1;
	}
}
