#include "pch.h"
#include "BERCoder.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Identifier.h"
#include "Length.h"
#include "Value.h"


BERCoder::BERCoder()
{
}


BERCoder::~BERCoder()
{
}

void BERCoder::setIdentifier(string pClass, string pComplexity, string pType)
{
	identifier.setClass(pClass);
	identifier.setComplexity(pComplexity);
	identifier.setTagFromType(pType);
}

void BERCoder::setIdentifier(string pClass, string pComplexity, unsigned int pTag)
{
	identifier.setClass(pClass);
	identifier.setComplexity(pComplexity);
	identifier.setTag(pTag);
}

void BERCoder::clearIdentifier()
{
	identifier.firstOctet = 0;
	identifier.longOctet.clear();
}

void BERCoder::setLength(unsigned long long pLength)
{
	length.setDefinedForm(pLength);
}

void BERCoder::clearLength()
{
	length.firstOctet = 0;
	length.longOctet.clear();
}

void BERCoder::setValue(string pValue)
{
	value.setValue(pValue);
}

void BERCoder::setValue(long long pValue, unsigned int pLength)
{
	value.setValue(pValue, pLength);
}

void BERCoder::clearValue()
{
	value.octet.clear();
	value.octetWord.clear();
}

stringstream BERCoder::concatAllValues(bool pIsValueNumber)
{
	stringstream concatedValue;

	concatedValue << hex << static_cast<int>(identifier.firstOctet) << " ";
	for (size_t i = 0; i < identifier.longOctet.size(); i++)
	{
		concatedValue << hex << static_cast<int>(identifier.longOctet.at(i)) << " ";
	}

	concatedValue << hex << static_cast<int>(length.firstOctet);
	for (size_t i = 0; i < length.longOctet.size(); i++)
	{
		concatedValue << " " << hex << static_cast<int>(length.longOctet.at(i));
	}

	if (pIsValueNumber)
	{
		for (size_t i = 0; i < value.octet.size(); i++)
		{
			concatedValue << " " << hex << static_cast<int>(value.octet.at(i));
		}
	}
	else
	{
		for (size_t i = 0; i < value.octetWord.size(); i++)
		{
			concatedValue << " " << hex << static_cast<int>(value.octetWord.at(i));
		}
	}
	return concatedValue;
}

stringstream BERCoder::encode(TreeNode* pNode, string pValue, vector<DataType> pVDataType, vector <Index> pVIndex, vector<Choice> pVChoice, vector<Sequence> pVSequence)
{
	clearIdentifier();
	clearLength();
	clearValue();

	bool isValueNumber;
	long long pValueINT = LONG_MIN;
	unsigned int byteCount, bitCount;
	stringstream encodedValue;

	//sprawdzanie czy wartosc jest liczba
	try
	{
		isValueNumber = true;
		pValueINT = stoll(pValue);
	}
	catch (const std::exception&)
	{
		isValueNumber = false;
	}

	//obliczanie dlugosci
	if (isValueNumber)
	{
		bitCount = ceil(log2(pValueINT));
		byteCount = ceil(bitCount / 8.0);
	}
	else
	{
		byteCount = pValue.size();
	}

	/*
	sprawdzenie poprawnosci podanej wartosci
	sprawdzic syntax
	1. typ danych odrazu w syntaxie
	- odczyt typu danych
	- sprawdzic rozmiar
	2. typ danych poza syntaxem
	- odpalac petle bo typ moze byc w DataType, Index, Choice, Sequence
	- jak juz znajdziemy to sprawdzac rozmiar
	*/

	/*
	sprawdzanie rozmiaru
	if (pVDataType.at(0).size != -1) //rozmiar w bajtach
	{
		//byteCount musi byc mniejsze od pVDataType.at(0).size
	}
	else if((pVDataType.at(0).sizeMin != -1) && (pVDataType.at(0).sizeMax != -1)) //rozmiar w zakresie sizeMin..sizeMax
	{
		//value musi sie miescic w zakresie sizeMin..sizeMax
	}
	else //brak ograniczen rozmiaru
	{
		//do nothing
	}
	*/
	
	//data type
	for (unsigned int i = 0; i < pVDataType.size(); i++)
	{
		if (pVDataType.at(i).name == pNode->syntax)
		{
			if (pVDataType.at(i).visibility.size() == 0) //kodowanie uniwersalne
			{
				setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).type);
				setLength(byteCount);
				if (isValueNumber)
				{
					setValue(pValueINT, byteCount);
				}
				else
				{
					setValue(pValue);
				}
				encodedValue = concatAllValues(isValueNumber);
				return encodedValue;
			}
			else
			{
				if (pVDataType.at(i).keyword == DATATYPE_KEYWORD_IMPLICIT)
				{
					if (pVDataType.at(i).visibility.size() == 0)//brak widocznosci klasy
					{
						setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).typeID);
					}
					else
					{
						setIdentifier(pVDataType.at(i).visibility, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).typeID);
					}

					setLength(byteCount);
					if (isValueNumber)
					{
						setValue(pValueINT, byteCount);
					}
					else
					{
						setValue(pValue);
					}
					encodedValue = concatAllValues(isValueNumber);
					return encodedValue;
				}
				else if (pVDataType.at(i).keyword == DATATYPE_KEYWORD_EXPLICIT)
				{
					if (pVDataType.at(i).visibility.size() == 0)//brak widocznosci klasy
					{
						setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(i).typeID);
					}
					else
					{
						setIdentifier(pVDataType.at(i).visibility, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(i).typeID);
					}
					encodedValue = concatAllValues(isValueNumber);
					return encodedValue;
				}

			}
		}
	}
	//typ danych integer - DataType::type
	//typ implicit - DataType::keyword
	//wartosc tagu 3 - DataType::typeID
	//ograniczenia rozmiaru - DataType::size lub sizeMin/sizeMax
	//nazwa - DataType::name
	//**widocznosc(klasa) Universal, application - DataType::visibility
}
