#include "pch.h"
#include "BERCoder.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "Regex.h"


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

int BERCoder::checkValue(string pValue, TreeNode* pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<SpecialDataType> &pVSpecialDataType)
{
	smatch result;
	regex_search(pNode->syntax, result, Regex::dataTypeEncoder());

	if (result[0].matched)//jest podstawowy typ
	{
		if (result[1].matched)//integer
		{
			if (!isValueNumber)
			{
				return -1;
			}
		}
		else if (result[2].matched)//octet string
		{
			//return 0;
		}
		else if (result[3].matched)//object identifier
		{
			//todo sprawdzenie poprawnosci
			//return 0;
		}
		else if (result[4].matched)//null
		{
			if (byteCount > 0)
			{
				return -2;
			}
		}

		for (size_t i = 0; i < pVSpecialDataType.size(); i++)//rozmiary z data type
		{
			if (pVSpecialDataType.at(i).name == pNode->name)
			{
				if (pVSpecialDataType.at(i).size != -1)
				{
					if (byteCount > pVSpecialDataType.at(i).size)
					{
						return -3;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					//todo zmienic regexa
					if (byteCount < pVSpecialDataType.at(i).sizeMin || byteCount > pVSpecialDataType.at(i).sizeMax)
					{
						return -4;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}
	else//mamy jakis inny typ
	{
		for (size_t i = 0; i < pVDataType.size(); i++)
		{
			if (pVSpecialDataType.at(i).name == pNode->name)
			{
				if (pVDataType.at(i).type == "INTEGER")//integer
				{
					if (!isValueNumber)
					{
						return -1;
					}
				}
				else if (pVDataType.at(i).type == "OCTET STRING")//octet string
				{
				}
				else if (pVDataType.at(i).type == "OBJECT IDENTIFIER")//object identifier
				{
					//todo sprawdzenie poprawnosci
				}
				else if (pVDataType.at(i).type == "NULL")//null
				{
					if (byteCount > 0)
					{
						return -2;
					}
				}

				if (pVDataType.at(i).size != -1) //rozmiar w bajtach
				{
					if (byteCount > pVDataType.at(i).size)
					{
						return -5;
					}
					else
					{
						return 0;
					}
				}
				else if ((pVDataType.at(0).sizeMin != -1) && (pVDataType.at(0).sizeMax != -1)) //rozmiar w zakresie sizeMin..sizeMax
				{
					if (byteCount < pVDataType.at(i).sizeMin || byteCount > pVDataType.at(i).sizeMax)
					{
						return -6;
					}
					else
					{
						return 0;
					}
				}
				else //brak ograniczen rozmiaru
				{
					return 0;
				}
			}
		}
	}
}

stringstream BERCoder::encode(TreeNode* pNode, string pValue, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<SpecialDataType> &pVSpecialDataType)
{
	clearIdentifier();
	clearLength();
	clearValue();

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
