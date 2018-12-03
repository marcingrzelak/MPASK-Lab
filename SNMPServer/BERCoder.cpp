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

	concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(identifier.firstOctet) << " ";
	for (size_t i = 0; i < identifier.longOctet.size(); i++)
	{
		concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(identifier.longOctet.at(i)) << " ";
	}

	concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(length.firstOctet);
	for (size_t i = 0; i < length.longOctet.size(); i++)
	{
		concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(length.longOctet.at(i));
	}

	if (pIsValueNumber)
	{
		for (size_t i = 0; i < value.octet.size(); i++)
		{
			concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(value.octet.at(i));
		}
	}
	else
	{
		for (size_t i = 0; i < value.octetWord.size(); i++)
		{
			concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(value.octetWord.at(i));
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
			if (pVDataType.at(i).name == pNode->syntax)
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

				for (size_t j = 0; j < pVSpecialDataType.size(); j++)
				{
					if (pVSpecialDataType.at(j).name == pNode->name)
					{
						if (pVSpecialDataType.at(j).size != -1) //rozmiar w bajtach
						{
							if (byteCount > pVSpecialDataType.at(j).size)
							{
								return -5;
							}
							else
							{
								return 0;
							}
						}
						else if ((pVSpecialDataType.at(j).sizeMin != -1) && (pVSpecialDataType.at(j).sizeMax != -1)) //rozmiar w zakresie sizeMin..sizeMax
						{
							if (byteCount < pVSpecialDataType.at(j).sizeMin || byteCount > pVSpecialDataType.at(j).sizeMax)
							{
								return -6;
							}
							else
							{
								return 0;
							}
						}
					}
				}
				if (pVDataType.at(i).size != -1) //rozmiar w bajtach
				{
					if (byteCount > pVDataType.at(i).size)
					{
						return -7;
					}
					else
					{
						return 0;
					}
				}
				else if ((pVDataType.at(i).sizeMin != -1) && (pVDataType.at(i).sizeMax != -1)) //rozmiar w zakresie sizeMin..sizeMax
				{
					if (isValueNumber)
					{
						if (pValueINT < pVDataType.at(i).sizeMin || pValueINT > pVDataType.at(i).sizeMax)
						{
							return -8;
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
					return 0;
				}
			}
		}
	}
}

string BERCoder::encode(TreeNode* pNode, string pValue, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<SpecialDataType> &pVSpecialDataType)
{
	clearIdentifier();
	clearLength();
	clearValue();

	stringstream encodedValue;
	int checkStatus;

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
		bitCount = floor(log2(abs(pValueINT)) + 1) + 1;
		byteCount = ceil(bitCount / 8.0);
	}
	else
	{
		byteCount = pValue.size();
	}

	checkStatus = checkValue(pValue, pNode, pVDataType, pVIndex, pVChoice, pVSequence, pVSpecialDataType);
	if (checkStatus == 0)
	{
		//data type
		for (unsigned int i = 0; i < pVDataType.size(); i++)
		{
			if (pVDataType.at(i).name == pNode->syntax)
			{
				if (pVDataType.at(i).keyword.size() == 0) //kodowanie uniwersalne
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
					return encodedValue.str();
				}
				else if (pVDataType.at(i).keyword == DATATYPE_KEYWORD_IMPLICIT)
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
					return encodedValue.str();
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
					return encodedValue.str();
				}
			}
		}
	}
	else
	{
	//wypisz blad
	return "Blad";
	}
}